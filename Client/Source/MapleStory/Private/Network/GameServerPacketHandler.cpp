#include "Network/GameServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "GameModes/MapleGameMode.h"


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
	if (Packet.player_infos_size() > 0) {
		for (const protocol::OtherPlayerInfo& PlayerInfo : Packet.player_infos()) {
			GameInstance->AddPlayer(PlayerInfo);
		}
	}
	return true;
}

bool FGameServerPacketHandler::HandleGameServerAddMonster(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddMonster& Packet) {
	if (Packet.mob_infos_size() > 0) {
		for (const protocol::MobInfo& MonsterInfo : Packet.mob_infos()) {
			GameInstance->AddMonster(MonsterInfo);
		}
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerRemoveObject(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerRemoveObject& Packet) {
	GameInstance->RemoveObject(Packet.object_id());
	return false;
}

bool FGameServerPacketHandler::HandleGameServerChangeMap(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChangeMap& Packet) {
	GameInstance->ChangeMap(Packet.map_id());
	return true;
}

bool FGameServerPacketHandler::HandleGameServerPlayerMove(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerMove& Packet) {
	GameInstance->UpdatePlayerPosition(Packet);
	return true;
}

bool FGameServerPacketHandler::HandleGameServerMobMove(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobMove& Packet) {
	return true;
}

bool FGameServerPacketHandler::HandleGameServerMobAttack(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobAttack& Packet) {
	return true;
}
