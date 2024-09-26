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
	
	
	if (Client) {
		bIsConnected = false;
		Client->CloseSocket();
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
		OtherPlayersQueue.Enqueue(OtherPlayerInfo);
	}
}

void UMapleGameInstance::AddMonster(const protocol::MobInfo& MonsterInfo) {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->AddMonster(MonsterInfo);
	} else {
		MonstersQueue.Enqueue(MonsterInfo);
	}
}

void UMapleGameInstance::RemoveObject(const int64 ObjectId) {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	const int8 Type = ObjectId / 1000000;

	switch (Type) {
		case 0: {
			if (GameMode) {
				GameMode->RemovePlayer(ObjectId);
			} else {
				RemovePlayerQueue.Enqueue(ObjectId);
			}
			break;
		}
		case 2: {
			if (GameMode) {
				GameMode->RemoveMonster(ObjectId);
			}
			break;
		}
	}
}

void UMapleGameInstance::UpdatePlayerPosition(const protocol::GameServerPlayerMove& Packet) const {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->UpdatePlayerPosition(Packet);
	}
}

void UMapleGameInstance::UpdateMonsterPosition(const protocol::GameServerMobMove& Packet) const {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->UpdateMonsterPosition(Packet);
	}
}

void UMapleGameInstance::SetMobAgro(const protocol::GameServerMobAgro& Packet) const {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->SetMobAgro(Packet);
	}
}

void UMapleGameInstance::RemoveMobAgro(const protocol::GameServerRemoveMobAgro& Packet) const {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->RemoveMobAgro(Packet);
	}
}

void UMapleGameInstance::PlayAttackAnimation(const protocol::GameServerMobAttack& Packet) const {
	AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->PlayAttackAnimation(Packet);
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
