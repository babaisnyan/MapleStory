#pragma once

#include "CoreMinimal.h"
#include "Data/Table/ItemTemplate.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemManager.generated.h"

UCLASS()
class MAPLESTORY_API UItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UItemManager();

public:
	FItemTemplate* GetItemTemplate(int32 Id);

private:
	UPROPERTY()
	TMap<int32, FItemTemplate> ItemData;
};
