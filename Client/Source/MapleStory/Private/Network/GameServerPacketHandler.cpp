﻿#include "Network/GameServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "Characters/MsLocalPlayer.h"
#include "Characters/MsPlayerBase.h"
#include "GameModes/MapleGameMode.h"
#include "UI/ChatWidget.h"


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
	GameInstance->UpdateMonsterPosition(Packet);
	return true;
}

bool FGameServerPacketHandler::HandleGameServerMobAttack(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobAttack& Packet) {
	GameInstance->PlayAttackAnimation(Packet);
	return true;
}

bool FGameServerPacketHandler::HandleGameServerPlayerDamage(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerDamage& Packet) {
	if (const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>()) {
		if (GameMode->Players.Contains(Packet.target_id())) {
			GameMode->Players[Packet.target_id()]->OnDamaged(Packet.damage());
		}
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerMobDamage(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobDamage& Packet) {
	return true;
}

bool FGameServerPacketHandler::HandleGameServerChat(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChat& Packet) {
	const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>();

	if (!GameMode) {
		return true;
	}

	if (!GameInstance->CurrentPlayer) {
		return true;
	}

	if (Packet.has_sender() && GameMode->Players.Contains(Packet.sender())) {
		AMsPlayerBase* Player = GameMode->Players[Packet.sender()];
		const FString Message = FString::Printf(TEXT("%s: %s"), *Player->Name, UTF8_TO_TCHAR(Packet.message().c_str()));

		GameInstance->CurrentPlayer->ChatWidget->AddChat(Message, Packet.type());
		Player->OnChat(Message);
	} else {
		GameInstance->CurrentPlayer->ChatWidget->AddChat(UTF8_TO_TCHAR(Packet.message().c_str()), Packet.type());
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerPlayerDead(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerDead& Packet) {
	if (GameInstance->CurrentPlayer->ObjectId == Packet.object_id()) {
		GameInstance->CurrentPlayer->OnDead();
		return true;
	}

	const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>();

	if (!GameMode) {
		return true;
	}

	if (GameMode->Players.Contains(Packet.object_id())) {
		GameMode->Players[Packet.object_id()]->OnDead();
	}

	return true;
}
