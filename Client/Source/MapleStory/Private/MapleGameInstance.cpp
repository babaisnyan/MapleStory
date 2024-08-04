// Fill out your copyright notice in the Description page of Project Settings.

#include "MapleGameInstance.h"

#include "SocketSubsystem.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "GameModes/MapleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Network/GameServerPacketHandler.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/PacketSession.h"

bool UMapleGameInstance::ConnectToLoginServer() {
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Login Client Socket"));
	Socket->SetNoDelay(true);

	FIPv4Address Ip;
	FIPv4Address::Parse(LoginIpAddress, Ip);

	const TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(Ip.Value);
	Address->SetPort(LoginPort);

	const bool bConnected = Socket->Connect(*Address);

	if (bConnected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connected to Login Server"));

		FLoginServerPacketHandler::Init(this);
		FGameServerPacketHandler::Init(this);

		Session = MakeShared<FPacketSession>(Socket, EServerType::Login);
		Session->Run();
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to Login Server"));
	}

	bIsConnected = bConnected;

	return bConnected;
}

void UMapleGameInstance::DisconnectFromServer() {
	bIsConnected = false;

	if (Session) {
		Session->Disconnect();
		Session.Reset();
		Session = nullptr;
	}

	if (Socket) {
		Socket->Close();

		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

bool UMapleGameInstance::ConnectToGameServer(const FString& IpAddress, const int16 Port) {
	DisconnectFromServer();
	
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Game Client Socket"));
	Socket->SetNoDelay(true);

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	const TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(Ip.Value);
	Address->SetPort(Port);

	const bool bConnected = Socket->Connect(*Address);

	if (bConnected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connected to Game Server"));
		
		Session = MakeShared<FPacketSession>(Socket, EServerType::Game);
		Session->Run();
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to Game Server"));
	}

	bIsConnected = bConnected;

	return bConnected;
}

void UMapleGameInstance::DisconnectFromGameServer() {
	if (Socket) {
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
		Session = nullptr;
	}
}

void UMapleGameInstance::SendPacket(const FSendBufferRef& SendBuffer) const {
	if (Socket == nullptr || Session == nullptr)
		return;

	Session->SendPacket(SendBuffer);
}

void UMapleGameInstance::HandleRecvPackets() const {
	if (!Socket || !Session) return;

	Session->HandleRecvPackets();
}

void UMapleGameInstance::Shutdown() {
	DisconnectFromServer();

	Super::Shutdown();
}

void UMapleGameInstance::QuitGame() {
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

	// switch (NewState) {
	// 	case ELoginState::Login:
	// 		break;
	// 	case ELoginState::CharacterSelection:
	// 		break;
	// 	case ELoginState::InGame:
	// 		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/MAP_Test"));
	// 		break;
	// }
}

void UMapleGameInstance::ChangeMap(const int32 NewMapId) {
	MapId = NewMapId;
	UGameplayStatics::OpenLevel(GetWorld(), *FString::Printf(TEXT("/Game/Maps/MAP_%d"), NewMapId));
}

void UMapleGameInstance::AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo) {
	 AMapleGameMode* GameMode = Cast<AMapleGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GameMode) {
		GameMode->AddPlayer(OtherPlayerInfo);
	}
}
