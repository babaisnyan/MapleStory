#include "Network/GameServerPacketHandler.h"

#include "MapleGameInstance.h"


bool HandleGameInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Invalid packet received"));
	return false;
}

bool HandleGameServerEnter(const FPacketSessionRef& Session, const protocol::GameServerEnter& Packet) {
	return true;
}
