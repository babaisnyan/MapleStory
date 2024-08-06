#include "Network/LoginServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/LoginCharacter.h"
#include "Characters/LoginController.h"
#include "GameModes/LoginGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Network/PacketCreator.h"
#include "UI/ChooseCharNameWindow.h"
#include "UI/LoginMessageWindow.h"

bool FLoginServerPacketHandler::HandleLoginInvalid(const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Invalid packet received"));
	return false;
}

bool FLoginServerPacketHandler::HandleLoginServerLogin(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerLogin& Packet) {
	static auto WindowClass = StaticLoadClass(ULoginMessageWindow::StaticClass(), nullptr, TEXT("/Game/UI/Login/WB_LoginNotice.WB_LoginNotice_C"));

	if (!WindowClass) {
		return false;
	}

	if (Packet.result() == protocol::LOGIN_RESULT_SUCCESS) {
		const UWorld* World = GameInstance->GetWorld();
		if (!World) {
			return false;
		}

		const auto SendBuffer = FPacketCreator::GetCharacterListRequest();
		GameInstance->SendPacket(SendBuffer);
	} else {
		ULoginMessageWindow* Window = CreateWidget<ULoginMessageWindow>(GameInstance.Get(), WindowClass);

		switch (Packet.result()) {
			case protocol::LOGIN_RESULT_INVALID_USERNAME:
				Window->ErrorCode = 1;
				break;
			case protocol::LOGIN_RESULT_INVALID_PASSWORD:
				Window->ErrorCode = 2;
				break;
			case protocol::LOGIN_RESULT_ALREADY_LOGGED_IN:
				Window->ErrorCode = 3;
				break;
			default:
				Window->ErrorCode = 4;
				break;
		}

		Window->AddToViewport();
	}

	return true;
}

bool FLoginServerPacketHandler::HandleLoginServerCharacterList(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerCharacterList& Packet) {
	const UWorld* World = GameInstance->GetWorld();
	if (!World) {
		return false;
	}

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(World));
	if (!GameMode) {
		return false;
	}

	const APlayerController* Controller = World->GetFirstPlayerController();
	if (!Controller) {
		return false;
	}

	const ALoginController* LoginController = Cast<ALoginController>(Controller);
	if (!LoginController) {
		return false;
	}

	LoginController->LoginWindow->SetVisibility(ESlateVisibility::Hidden);
	GameInstance->ChangeLoginState(ELoginState::CharacterSelection);

	if (Packet.characters_size() == 0) return true;

	if (Packet.characters_size() >= 6) {
		GameMode->bCanCreateCharacter = false;
	}

	GameMode->LoginCharacterVisualizer->ClearAvatars();
	GameMode->LoginCharacters.Empty();

	for (int32 i = 0; i < Packet.characters_size(); i++) {
		const protocol::LoginCharacter Character = Packet.characters(i);
		const FString Name = UTF8_TO_TCHAR(Character.name().c_str());

		GameMode->LoginCharacterVisualizer->SetAvatar(i, static_cast<EAvatarType>(Character.type()), Name);
		GameMode->LoginCharacters.Add(Character);
	}

	return true;
}

bool FLoginServerPacketHandler::HandleLoginServerDeleteCharacter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerDeleteCharacter& Packet) {
	static auto WindowClass = StaticLoadClass(ULoginMessageWindow::StaticClass(), nullptr, TEXT("/Game/UI/Login/WB_LoginNotice.WB_LoginNotice_C"));

	const UWorld* World = GameInstance->GetWorld();
	if (!World) {
		return false;
	}

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(World));
	if (!GameMode) {
		return false;
	}

	if (Packet.success()) {
		GameMode->DeleteCharacter(Packet.character_id());
	} else {
		ULoginMessageWindow* Window = CreateWidget<ULoginMessageWindow>(GameInstance.Get(), WindowClass);
		Window->ErrorCode = 4;
		Window->AddToViewport();
	}

	return true;
}

bool FLoginServerPacketHandler::HandleLoginServerCreateCharacter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerCreateCharacter& Packet) {
	UWorld* World = GameInstance->GetWorld();
	if (!World) {
		return false;
	}

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(UGameplayStatics::GetGameMode(World));
	if (!GameMode) {
		return false;
	}

	static auto WindowClass = StaticLoadClass(ULoginMessageWindow::StaticClass(), nullptr, TEXT("/Game/UI/Login/WB_LoginNotice.WB_LoginNotice_C"));
	ULoginMessageWindow* Window = CreateWidget<ULoginMessageWindow>(GameInstance.Get(), WindowClass);

	switch (Packet.result()) {
		case protocol::CREATE_CHAR_RESULT_SUCCESS:
			if (Packet.has_character()) {
				const FString Name = UTF8_TO_TCHAR(Packet.character().name().c_str());

				TArray<UUserWidget*> FoundWidgets;
				UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, FoundWidgets, UChooseCharNameWindow::StaticClass(), false);

				if (!FoundWidgets.IsEmpty()) {
					GameMode->LoginCharacterVisualizer->SetLastAvatar(static_cast<EAvatarType>(Packet.character().type()), Name);
					GameMode->LoginCharacters.Add(Packet.character());

					FoundWidgets[0]->RemoveFromParent();
					GameInstance->ChangeLoginState(ELoginState::CharacterSelection);
				}
			}
			break;
		case protocol::CREATE_CHAR_RESULT_INVALID_NAME:
			Window->ErrorCode = 5;
			Window->AddToViewport();
			break;
		case protocol::CREATE_CHAR_RESULT_NAME_TAKEN:
			Window->ErrorCode = 7;
			Window->AddToViewport();
			break;
		case protocol::CREATE_CHAR_RESULT_NO_SLOTS:
			Window->ErrorCode = 6;
			Window->AddToViewport();
			break;
		default:
			break;
	}

	return true;
}

bool FLoginServerPacketHandler::HandleLoginServerCharSelectResult(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerCharSelectResult& Packet) {
	static auto WindowClass = StaticLoadClass(ULoginMessageWindow::StaticClass(), nullptr, TEXT("/Game/UI/Login/WB_LoginNotice.WB_LoginNotice_C"));
	ULoginMessageWindow* Window = CreateWidget<ULoginMessageWindow>(GameInstance.Get(), WindowClass);

	if (Packet.result() == protocol::SELECT_CHAR_RESULT_SUCCESS) {
		if (!Packet.has_ip() || !Packet.has_port() || !Packet.has_auth_key()) {
			Window->ErrorCode = 4;
			Window->AddToViewport();
			return false;
		}

		const FString IpAddress = UTF8_TO_TCHAR(Packet.ip().c_str());
		const auto SendBuffer = FPacketCreator::GetClientEnterRequest(Packet.character_id(), Packet.auth_key());
		GameInstance->ClearSendQueue();
		GameInstance->EnqueueSendPacket(SendBuffer);
		GameInstance->ConnectToGameServer(IpAddress, Packet.port());
	}

	return true;
}
