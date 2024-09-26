#pragma once

#include "CoreMinimal.h"
#include "game_protocol.pb.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"
#include "GameFramework/GameModeBase.h"
#include "MapleGameMode.generated.h"


class AMonster;

namespace protocol {
	class GameServerPlayerMove;
}

class AMsPlayerBase;
class AMsPlayer;

UCLASS()
class MAPLESTORY_API AMapleGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	AMapleGameMode();

	void AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo);
	void RemovePlayer(int64 ObjectId);
	void UpdatePlayerPosition(const protocol::GameServerPlayerMove& MovePacket);

	void AddMonster(const protocol::MobInfo& MonsterInfo);
	void RemoveMonster(int64 ObjectId);
	void UpdateMonsterPosition(const protocol::GameServerMobMove& Packet);
	void SetMobAgro(const protocol::GameServerMobAgro& Packet);
	void RemoveMobAgro(const protocol::GameServerRemoveMobAgro& Packet);
	void PlayAttackAnimation(const protocol::GameServerMobAttack& Packet);

	uint64_t GetExpForLevel(int32 Level) const;

protected:
	virtual void BeginPlay() override;

private:
	void InitExpTables();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Avatar")
	EAvatarType AvatarType = EAvatarType::GoTalk;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	TMap<int64, TObjectPtr<AMsPlayerBase>> Players = {};

	UPROPERTY(VisibleAnywhere, Category = "Monster")
	TMap<int64, TObjectPtr<AMonster>> Monsters = {};

private:
	FVector PlayerStartLocation;

	uint64 ExpTable[300];
};
