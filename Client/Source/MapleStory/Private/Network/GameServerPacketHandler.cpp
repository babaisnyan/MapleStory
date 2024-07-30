#include "Network/GameServerPacketHandler.h"

#include "AITestsCommon.h"
#include "MapleGameInstance.h"
#include "GameModes/MapleGameMode.h"
#include "Kismet/GameplayStatics.h"


bool HandleGameInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Invalid packet received"));
	return false;
}

bool HandleGameServerEnter(const FPacketSessionRef& Session, const protocol::GameServerEnter& Packet) {
	if (Packet.has_player_info() && Packet.has_map_id()) {
		FGameServerPacketHandler::GameInstance->PlayerInfoTemp = Packet.player_info();
		FGameServerPacketHandler::GameInstance->ChangeMap(Packet.map_id());
	}
	return true;
}

bool HandleGameServerAddPlayer(const FPacketSessionRef& Session, const protocol::GameServerAddPlayer& Packet) {
	return true;
}

bool HandleGameServerChangeMap(const FPacketSessionRef& Session, const protocol::GameServerChangeMap& Packet) {
	FGameServerPacketHandler::GameInstance->ChangeMap(Packet.map_id());
	return true;
}
