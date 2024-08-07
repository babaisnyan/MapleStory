#pragma once

#include "CoreMinimal.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"
#include "GameFramework/GameModeBase.h"
#include "MapleGameMode.generated.h"


class AMsPlayerBase;
class AMsPlayer;

UCLASS()
class MAPLESTORY_API AMapleGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	AMapleGameMode();

	void AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo);
	void RemovePlayer(int32 PlayerId);

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Avatar")
	EAvatarType AvatarType = EAvatarType::GoTalk;

private:
	UPROPERTY(VisibleAnywhere, Category = "Player")
	TMap<int32, TObjectPtr<AMsPlayerBase>> OtherPlayers = {};
};
