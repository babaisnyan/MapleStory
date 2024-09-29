#include "UI/ChatWidget.h"

#include "MapleGameInstance.h"
#include "MapleStory.h"
#include "Characters/MsLocalPlayer.h"
#include "Components/EditableTextBox.h"
#include "Components/Overlay.h"
#include "Components/ScrollBox.h"
#include "Network/PacketCreator.h"
#include "Slate/SceneViewport.h"
#include "UI/ChatHistory.h"

void UChatWidget::AddChat(const FString& InText, const protocol::ChatType ChatType) const {
	if (ChatHistoryClass) {
		UChatHistory* ChatHistory = CreateWidget<UChatHistory>(GetWorld(), ChatHistoryClass);

		if (!ChatHistory) {
			return;
		}

		switch (ChatType) {
			case protocol::CHAT_TYPE_NORMAL:
				ChatHistory->SetText(InText);
				break;
			case protocol::CHAT_TYPE_SYSTEM:
				ChatHistory->SetText(InText, FColor::Transparent, FColor(0.8f, 0.8f, 0.8f, 1.0f));
				break;
			default:
				return;
		}

		ChatHistoryBox->AddChild(ChatHistory);
		ChatHistoryBox->ScrollToEnd();
	}
}

void UChatWidget::ToggleChat() {
	HistoryPanel->SetVisibility(HistoryPanel->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	ChatTextBox->SetIsEnabled(HistoryPanel->IsVisible());

	if (HistoryPanel->IsVisible()) {
		ChatTextBox->SetFocus();
	} else {
		ChatTextBox->SetText(FText::GetEmpty());

	}
}

void UChatWidget::SendChat(const FString& InText) {
	if (InText.IsEmpty() || InText.TrimStartAndEnd().IsEmpty()) {
		return;
	}

	if (InText.Len() > 35) {
		return;
	}

	SEND_PACKET(FPacketCreator::GetChat(InText));

	const auto GameInstance = GetGameInstance<UMapleGameInstance>();

	if (!GameInstance || !GameInstance->CurrentPlayer) {
		return;
	}

	AddChat(FString::Printf(TEXT("%s: %s"), *GameInstance->CurrentPlayer->Name, *InText));
	GameInstance->CurrentPlayer->OnChat(FString::Printf(TEXT("%s: %s"), *GameInstance->CurrentPlayer->Name, *InText));
	
	if (APlayerController* PlayerController = GetOwningPlayer()) {
		const FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}
