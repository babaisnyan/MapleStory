// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWindow.h"

#include "MapleGameInstance.h"
#include "login_protocol.pb.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/PacketCreator.h"

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

	const auto SendBuffer = FPacketCreator::GetLoginRequest(Username, Password);
	GameInstance->SendPacket(SendBuffer);

	PasswordTextBox->SetText(FText::FromString(TEXT("")));
	return true;
}
