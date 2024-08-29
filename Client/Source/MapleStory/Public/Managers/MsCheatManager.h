#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "MsCheatManager.generated.h"

UCLASS()
class MAPLESTORY_API UMsCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void SpawnMob(int32 MobId) const;

	UFUNCTION(Exec)
	void LogMobList() const;

	UFUNCTION(Exec)
	void MobSpawnTest() const;
};
