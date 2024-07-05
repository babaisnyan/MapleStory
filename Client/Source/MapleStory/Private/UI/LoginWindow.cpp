// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWindow.h"

#include "MapleGameInstance.h"
#include "login_protocol.pb.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Network/LoginServerPacketHandler.h"

bool ULoginWindow::TryLogin() {
	const auto GameInstance = Cast<UMapleGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameInstance) {
		return false;
	}

	if (!GameInstance->bIsConnected) {
		const bool bConnected = GameInstance->ConnectToLoginServer();

		if (!bConnected) {
			return false;
		}
	}

	if (!UsernameTextBox || !PasswordTextBox) {
		return false;
	}

	const FString Username = UsernameTextBox->GetText().ToString();
	const FString Password = PasswordTextBox->GetText().ToString();

	if (Username.IsEmpty() || Password.IsEmpty()) {
		return false;
	}

	protocol::LoginClientLogin Packet;
	Packet.set_username(TCHAR_TO_UTF8(*Username));
	Packet.set_password(TCHAR_TO_UTF8(*Password));

	const auto SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet);
	GameInstance->SendPacket(SendBuffer);
	return true;
}
