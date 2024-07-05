#include "Network/LoginServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Characters/LoginController.h"

bool HandleLoginInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
	UE_LOG(LogTemp, Warning, TEXT("Invalid packet received"));
	return false;
}

bool HandleLoginServerLogin(const FPacketSessionRef& Session, const protocol::LoginServerLogin& Packet) {
	if (Packet.result() == protocol::Success) {
		if(!FLoginServerPacketHandler::GameInstance) {
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
		FLoginServerPacketHandler::GameInstance->ChangeLoginState(ELoginState::InGame);
	} else {
		const int8 ErrorCode = Packet.result();

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Login failed with error code: %d"), ErrorCode));
	}

	return true;
}

bool HandleLoginServerChat(const FPacketSessionRef& Session, const protocol::LoginServerChat& Packet) {
	const auto Message = Packet.message();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Chat: %s"), *FString(Message.c_str())));
	return true;
}
