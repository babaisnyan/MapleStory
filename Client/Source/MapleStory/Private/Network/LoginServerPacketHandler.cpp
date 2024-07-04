#include "Network/LoginServerPacketHandler.h"

bool HandleLoginInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
	UE_LOG(LogTemp, Warning, TEXT("Invalid packet received"));
	return false;
}

bool HandleLoginServerLogin(const FPacketSessionRef& Session, const protocol::LoginServerLogin& Packet) {
	return true;
}

bool HandleLoginServerChat(const FPacketSessionRef& Session, const protocol::LoginServerChat& Packet) {
	const auto Message = Packet.message();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Chat: %s"), *FString(Message.c_str())));
	return true;
}
