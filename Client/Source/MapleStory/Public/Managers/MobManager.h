#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "Data/Table/MobTemplate.h"

#include "MobManager.generated.h"

UCLASS()
class MAPLESTORY_API UMobManager : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UMobManager();

public:
	FMobTemplate* GetMobTemplate(int32 Id);

private:
	UPROPERTY()
	TMap<int32, FMobTemplate> MobData;
};
