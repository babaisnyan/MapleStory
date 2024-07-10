#include "Network/LoginServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Characters/LoginCharacter.h"
#include "Characters/LoginController.h"
#include "GameModes/LoginGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Network/PacketCreator.h"

bool HandleLoginInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Invalid packet received"));
	return false;
}

bool HandleLoginServerLogin(const FPacketSessionRef& Session, const protocol::LoginServerLogin& Packet) {
	if (Packet.result() == protocol::Success) {
		if (!FLoginServerPacketHandler::GameInstance) {
			return false;
		}

		const UWorld* World = FLoginServerPacketHandler::GameInstance->GetWorld();
		if (!World) {
			return false;
		}

		const auto SendBuffer = FPacketCreator::GetCharacterListRequest();
		FLoginServerPacketHandler::GameInstance->SendPacket(SendBuffer);
	} else {
		const int8 ErrorCode = Packet.result();
		//TODO: 오류 메시지 창 띄우기
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login failed with error code: %d"), ErrorCode));
	}

	return true;
}

bool HandleLoginServerCharacterList(const FPacketSessionRef& Session, const protocol::LoginServerCharacterList& Packet) {
	if (!FLoginServerPacketHandler::GameInstance) {
		return false;
	}

	const UWorld* World = FLoginServerPacketHandler::GameInstance->GetWorld();
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
	FLoginServerPacketHandler::GameInstance->ChangeLoginState(ELoginState::CharacterSelection);

	if (Packet.characters_size() == 0) return true;

	if (Packet.characters_size() >= 6) {
		GameMode->bCanCreateCharacter = false;
	}

	for (int32 i = 0; i < Packet.characters_size(); i++) {
		const protocol::LoginCharacter Character = Packet.characters(i);
		const FString Name = UTF8_TO_TCHAR(Character.name().c_str());

		GameMode->LoginCharacterVisualizer->SetAvatar(i, static_cast<EAvatarType>(Character.type()), Name);
		GameMode->LoginCharacters.Add(Character);
	}


	return true;
}

bool HandleLoginServerChat(const FPacketSessionRef& Session, const protocol::LoginServerChat& Packet) {
	const auto Message = Packet.message();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Chat: %s"), *FString(Message.c_str())));
	return true;
}
