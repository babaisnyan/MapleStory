#include "UI/LoginWindow.h"

#include "MapleGameInstance.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Network/PacketCreator.h"

bool ULoginWindow::TryLogin() {
	const auto GameInstance = Cast<UMapleGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameInstance) {
		return false;
	}

	if (!UsernameTextBox || !PasswordTextBox) {
		return false;
	}

	const FString Username = UsernameTextBox->GetText().ToString();
	const FString Password = PasswordTextBox->GetText().ToString();

	if (Username.IsEmpty() || Password.IsEmpty()) {
		return false;
	}

	if (GameInstance->bIsConnected) {
		GameInstance->DisconnectFromServer();
	}

	const auto SendBuffer = FPacketCreator::GetLoginRequest(Username, Password);
	GameInstance->ClearSendQueue();
	GameInstance->EnqueueSendPacket(SendBuffer);
	GameInstance->ConnectToLoginServer();
	PasswordTextBox->SetText(FText::FromString(TEXT("")));
	return true;
}
