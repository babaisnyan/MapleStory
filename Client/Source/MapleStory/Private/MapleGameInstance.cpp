// Fill out your copyright notice in the Description page of Project Settings.

#include "MapleGameInstance.h"

#include "SocketSubsystem.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/PacketSession.h"

bool UMapleGameInstance::ConnectToLoginServer() {
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Login Client Socket"));
	Socket->SetNoDelay(true);

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	const TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(Ip.Value);
	Address->SetPort(LoginPort);

	const bool bConnected = Socket->Connect(*Address);

	if (bConnected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connected to Login Server"));

		FLoginServerPacketHandler::Init(this);
		Session = MakeShared<FPacketSession>(Socket);
		Session->Run();
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to Login Server"));
	}

	bIsConnected = bConnected;

	return bConnected;
}

void UMapleGameInstance::DisconnectFromLoginServer() {
	if (Socket) {
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
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
	Super::Shutdown();

	if (Session) {
		Session->Disconnect();
	}

	DisconnectFromLoginServer();
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

	switch (NewState) {
		case ELoginState::Login:
			break;
		case ELoginState::CharacterSelection:
			break;
		case ELoginState::InGame:
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/MAP_Test"));
			break;
	}
}
