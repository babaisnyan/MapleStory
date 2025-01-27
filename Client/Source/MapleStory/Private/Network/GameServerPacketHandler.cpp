﻿#include "Network/GameServerPacketHandler.h"

#include "MapleGameInstance.h"
#include "Actors/Monster.h"
#include "Characters/MsLocalPlayer.h"
#include "Characters/MsPlayerBase.h"
#include "Components/PlayerStatComponent.h"
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
	if (const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>()) {
		if (GameMode->Monsters.Contains(Packet.target_id())) {
			GameMode->Monsters[Packet.target_id()]->OnDamaged(Packet.damage(), Packet.is_critical());
		}
	}

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

bool FGameServerPacketHandler::HandleGameServerRevive(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerRevive& Packet) {
	if (GameInstance->CurrentPlayer->ObjectId == Packet.object_id()) {
		GameInstance->CurrentPlayer->OnRevive();
		return true;
	}

	const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>();

	if (!GameMode) {
		return true;
	}

	if (GameMode->Players.Contains(Packet.object_id())) {
		GameMode->Players[Packet.object_id()]->OnRevive();
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerTeleportPlayer(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerTeleportPlayer& Packet) {
	if (GameInstance->CurrentPlayer->ObjectId == Packet.object_id()) {
		GameInstance->CurrentPlayer->Teleport(Packet.x(), Packet.y());
		return true;
	}

	const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>();

	if (!GameMode) {
		return true;
	}

	if (GameMode->Players.Contains(Packet.object_id())) {
		GameMode->Players[Packet.object_id()]->Teleport(Packet.x(), Packet.y());
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerUpdatePlayerStat(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerUpdatePlayerStat& Packet) {
	if (GameInstance->CurrentPlayer) {
		if (Packet.has_level()) {
			GameInstance->CurrentPlayer->PlayerStat->Level = Packet.level();
		}

		if (Packet.has_exp()) {
			GameInstance->CurrentPlayer->PlayerStat->Exp = Packet.exp();
		}

		if (Packet.has_hp()) {
			GameInstance->CurrentPlayer->PlayerStat->Hp = Packet.hp();
		}

		if (Packet.has_mp()) {
			GameInstance->CurrentPlayer->PlayerStat->Mp = Packet.mp();
		}

		if (Packet.has_max_hp()) {
			GameInstance->CurrentPlayer->PlayerStat->MaxHp = Packet.max_hp();
		}

		if (Packet.has_max_mp()) {
			GameInstance->CurrentPlayer->PlayerStat->MaxMp = Packet.max_mp();
		}

		if (Packet.has_str()) {
			GameInstance->CurrentPlayer->PlayerStat->Str = Packet.str();
		}

		if (Packet.has_dex()) {
			GameInstance->CurrentPlayer->PlayerStat->Dex = Packet.dex();
		}

		if (Packet.has_int_()) {
			GameInstance->CurrentPlayer->PlayerStat->Int = Packet.int_();
		}

		if (Packet.has_luk()) {
			GameInstance->CurrentPlayer->PlayerStat->Luk = Packet.luk();
		}

		if (Packet.has_ap()) {
			GameInstance->CurrentPlayer->PlayerStat->Ap = Packet.ap();
		}

		if (Packet.has_sp()) {
			GameInstance->CurrentPlayer->PlayerStat->Sp = Packet.sp();
		}

		GameInstance->CurrentPlayer->UpdateStatusBar();
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerAttack(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAttack& Packet) {
	const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>();

	if (!GameMode) {
		return true;
	}

	if (GameMode->Players.Contains(Packet.object_id())) {
		GameMode->Players[Packet.object_id()]->OnAttack();
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerPlayerLevelUp(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerLevelUp& Packet) {
	const auto GameMode = GameInstance->GetWorld()->GetAuthGameMode<AMapleGameMode>();

	if (!GameMode) {
		return true;
	}

	if (Packet.object_id() == GameInstance->CurrentPlayer->ObjectId) {
		GameInstance->CurrentPlayer->OnLevelUp();
	} else {
		GameMode->Players[Packet.object_id()]->OnLevelUp();
	}

	return true;
}

bool FGameServerPacketHandler::HandleGameServerAddExpMessage(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddExpMessage& Packet) {
	return true;
}

bool FGameServerPacketHandler::HandleGameServerAddMeso(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddMeso& Packet) {
	if (GameInstance->CurrentPlayer) {
		GameInstance->CurrentPlayer->Meso += Packet.meso();
	}

	return true;
}
