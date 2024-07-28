#include "Network/GameServerPacketHandler.h"

#include "MapleGameInstance.h"


bool HandleGameInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Invalid packet received"));
	return false;
}

bool HandleGameServerEnter(const FPacketSessionRef& Session, const protocol::GameServerEnter& Packet) {
	FGameServerPacketHandler::GameInstance->ChnageMap(Packet.map_id());
	return true;
}

bool HandleGameServerAddPlayer(const FPacketSessionRef& Session, const protocol::GameServerAddPlayer& Packet) {
	return true;
}

bool HandleGameServerChangeMap(const FPacketSessionRef& Session, const protocol::GameServerChangeMap& Packet) {
	FGameServerPacketHandler::GameInstance->ChnageMap(Packet.map_id());
	return true;
}
