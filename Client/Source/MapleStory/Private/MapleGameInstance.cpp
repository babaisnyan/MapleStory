#include "MapleGameInstance.h"

#include "GameModes/MapleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Network/GameServerPacketHandler.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/TCPClientComponent.h"

void UMapleGameInstance::Init() {
	Super::Init();

	const auto WeakThis = MakeWeakObjectPtr(this);
	FLoginServerPacketHandler::Init(WeakThis);
	FGameServerPacketHandler::Init(WeakThis);
}

void UMapleGameInstance::BeginDestroy() {
	Super::BeginDestroy();

	if (bIsConnected) {
		DisconnectFromServer();
	}
}

void UMapleGameInstance::ConnectToLoginServer() {
	Client = NewObject<UTCPClientComponent>();
	Client->bAutoReconnectOnSendFailure = false;
	Client->bReceiveDataOnGameThread = true;
	Client->bShouldAutoConnectOnBeginPlay = false;
	Client->OnConnected.AddDynamic(this, &UMapleGameInstance::OnLoginServerConnected);
	Client->OnReceivedBytes.AddDynamic(this, &UMapleGameInstance::OnReceivedBytesLogin);
	Client->ConnectToSocketAsClient(LoginIpAddress, LoginPort);
}

void UMapleGameInstance::DisconnectFromServer() {
	bIsConnected = false;
	Client->CloseSocket();
}

void UMapleGameInstance::ConnectToGameServer(const FString& IpAddress, const int16 Port) {
	DisconnectFromServer();

	Client = NewObject<UTCPClientComponent>();
	Client->bAutoReconnectOnSendFailure = false;
	Client->bReceiveDataOnGameThread = true;
	Client->bShouldAutoConnectOnBeginPlay = false;
	Client->OnConnected.AddDynamic(this, &UMapleGameInstance::OnGameServerConnected);
	Client->OnReceivedBytes.AddDynamic(this, &UMapleGameInstance::OnReceivedBytesGame);
	Client->ConnectToSocketAsClient(IpAddress, Port);
}

void UMapleGameInstance::EnqueueSendPacket(const FSendBufferRef& SendBuffer) {
	SendQueue.Enqueue(SendBuffer);
}

void UMapleGameInstance::ClearSendQueue() {
	SendQueue.Empty();
}

void UMapleGameInstance::SendPacket(const FSendBufferRef& SendBuffer) const {
	if (Client == nullptr || !SendBuffer.IsValid() || !Client->IsConnected()) {
		return;
	}

	Client->Emit(SendBuffer->GetBufferArray());
}

void UMapleGameInstance::Shutdown() {
	Super::Shutdown();
}

void UMapleGameInstance::QuitGame() const {
	const UWorld* World = GetWorld();
	if (World) {
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController) {
			UKismetSystemLibrary::QuitGame(World, PlayerController, EQuitPreference::Quit, true);
		}
	}
}

void UMapleGameInstance::ChangeLoginState(const ELoginState NewState) {
	LoginState = NewState;
}

void UMapleGameInstance::ChangeMap(const int32 NewMapId) {
	MapId = NewMapId;
	UGameplayStatics::OpenLevel(GetWorld(), *FString::Printf(TEXT("/Game/Maps/MAP_%d"), NewMapId));
}

void UMapleGameInstance::AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo) {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->AddPlayer(OtherPlayerInfo);
	} else {
		protocol::OtherPlayerInfo OtherPlayerInfoCopy = OtherPlayerInfo;
		OtherPlayersQueue.Enqueue(OtherPlayerInfoCopy);
	}
}

void UMapleGameInstance::RemovePlayer(const int32 PlayerId) {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->RemovePlayer(PlayerId);
	} else {
		RemovePlayerQueue.Enqueue(PlayerId);
	}
}

void UMapleGameInstance::OnLoginServerConnected() {
	Client->OnConnected.RemoveDynamic(this, &UMapleGameInstance::OnLoginServerConnected);
	bIsConnected = true;

	while (!SendQueue.IsEmpty()) {
		FSendBufferRef SendBuffer;
		SendQueue.Dequeue(SendBuffer);
		SendPacket(SendBuffer);
	}
}

void UMapleGameInstance::OnReceivedBytesLogin(const TArray<uint8>& Bytes) {
	FLoginServerPacketHandler::HandlePacket(Client, Bytes.GetData(), Bytes.Num());
}

void UMapleGameInstance::OnGameServerConnected() {
	Client->OnConnected.RemoveDynamic(this, &UMapleGameInstance::OnGameServerConnected);
	bIsConnected = true;

	while (!SendQueue.IsEmpty()) {
		FSendBufferRef SendBuffer;
		SendQueue.Dequeue(SendBuffer);
		SendPacket(SendBuffer);
	}
}

void UMapleGameInstance::OnReceivedBytesGame(const TArray<uint8>& Bytes) {
	FGameServerPacketHandler::HandlePacket(Client, Bytes.GetData(), Bytes.Num());
}
