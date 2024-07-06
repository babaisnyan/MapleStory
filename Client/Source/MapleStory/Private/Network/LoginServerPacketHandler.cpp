#include "Network/LoginServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Characters/LoginController.h"
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

		const APlayerController* Controller = World->GetFirstPlayerController();
		if (!Controller) {
			return false;
		}

		const ALoginController* LoginController = Cast<ALoginController>(Controller);
		if (!LoginController) {
			return false;
		}

		LoginController->LoginWindow->RemoveFromParent();
		FLoginServerPacketHandler::GameInstance->ChangeLoginState(ELoginState::CharacterSelection);

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
	if (Packet.characters_size() == 0) return true;
	
	for (auto&& Character : Packet.characters()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Character: %s"), *FString(UTF8_TO_TCHAR(Character.name().c_str()))));
	}
	
	return true;
}

bool HandleLoginServerChat(const FPacketSessionRef& Session, const protocol::LoginServerChat& Packet) {
	const auto Message = Packet.message();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Chat: %s"), *FString(Message.c_str())));
	return true;
}
