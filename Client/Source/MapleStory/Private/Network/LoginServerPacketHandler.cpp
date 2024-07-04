#include "Network/LoginServerPacketHandler.h"

bool HandleLoginServerChat(const FPacketSessionRef& Session, const protocol::LoginServerChat& Packet) {
	auto Message = Packet.message();
	UE_LOG(LogTemp, Warning, TEXT("Chat message: %s"), *FString(UTF8_TO_TCHAR(Message.c_str())));
	return true;
}
