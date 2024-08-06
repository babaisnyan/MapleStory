#include "Network/GameServerPacketHandler.h"

#include "MapleGameInstance.h"


bool FGameServerPacketHandler::HandleGameInvalid(const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Invalid packet received"));
	return false;
}

bool FGameServerPacketHandler::HandleGameServerEnter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerEnter& Packet) {
	if (Packet.has_player_info() && Packet.has_map_id()) {
		GameInstance->PlayerInfoTemp = std::move(Packet.player_info());
		GameInstance->ChangeMap(Packet.map_id());
	}
	return true;
}

bool FGameServerPacketHandler::HandleGameServerAddPlayer(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddPlayer& Packet) {
	if (Packet.has_player_info()) {
		GameInstance->AddPlayer(Packet.player_info());
	}
	return true;
}

bool FGameServerPacketHandler::HandleGameServerChangeMap(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChangeMap& Packet) {
	GameInstance->ChangeMap(Packet.map_id());
	return true;
}
