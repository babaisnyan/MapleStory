// Fill out your copyright notice in the Description page of Project Settings.

#include "MapleGameInstance.h"

#include "SocketSubsystem.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "Network/PacketSession.h"
#include "Serialization/ArrayWriter.h"

void UMapleGameInstance::ConnectToLoginServer() {
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Login Client Socket"));

	FIPv4Address Ip;
	FIPv4Address::Parse(IpAddress, Ip);

	const TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(Ip.Value);
	Address->SetPort(LoginPort);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Connecting to %s:%d"), *IpAddress, LoginPort));

	const bool bConnected = Socket->Connect(*Address);
	
	if (bConnected) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Connected to Login Server"));

		Session = MakeShared<FPacketSession>(Socket);
		Session->Run();
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to Login Server"));
	}
}

void UMapleGameInstance::DisconnectFromLoginServer() {
	if (Socket) {
		ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

void UMapleGameInstance::SendPacket(FSendBufferRef SendBuffer) {
}

void UMapleGameInstance::HandleRecvPackets() const {
	if (!Socket || !Session) return;

	Session->HandleRecvPackets();
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
