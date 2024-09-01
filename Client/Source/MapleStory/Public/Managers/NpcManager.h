#pragma once

#include "CoreMinimal.h"
#include "Data/Table/NpcTemplate.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NpcManager.generated.h"

UCLASS()
class MAPLESTORY_API UNpcManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UNpcManager();

public:
	const FNpcTemplate* GetNpcTemplate(int32 Id) const;

private:
	UPROPERTY()
	TMap<int32, FNpcTemplate> NpcData;
};
