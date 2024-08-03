// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"

#include "GameFramework/GameModeBase.h"

#include "MapleGameMode.generated.h"


class AMsPlayer;

UCLASS()
class MAPLESTORY_API AMapleGameMode : public AGameModeBase {
	GENERATED_BODY()

public:
	AMapleGameMode();

protected:
	virtual void BeginPlay() override;
	void AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Avatar")
	EAvatarType AvatarType = EAvatarType::GoTalk;

private:
	TMap<EAvatarType, TSubclassOf<AMsPlayer>> PlayerClasses = {};

	UPROPERTY(VisibleAnywhere, Category = "Player")
	TMap<int32, TObjectPtr<AMsPlayer>> OtherPlayers = {};
};
