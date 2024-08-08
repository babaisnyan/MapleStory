﻿#include "Network/GameServerPacketHandler.h"

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
	if (Packet.player_infos_size() > 0) {
		for (const protocol::OtherPlayerInfo& PlayerInfo : Packet.player_infos()) {
			GameInstance->AddPlayer(PlayerInfo);
		}
	}
	return true;
}

bool FGameServerPacketHandler::HandleGameServerRemovePlayer(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerRemovePlayer& Packet) {
	GameInstance->RemovePlayer(Packet.player_id());
	return true;
}

bool FGameServerPacketHandler::HandleGameServerRemoveObject(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerRemoveObject& Packet) {
	return false;
}

bool FGameServerPacketHandler::HandleGameServerChangeMap(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChangeMap& Packet) {
	GameInstance->ChangeMap(Packet.map_id());
	return true;
}
