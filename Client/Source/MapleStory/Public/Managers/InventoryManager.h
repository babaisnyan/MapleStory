#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryManager.generated.h"

class UItem;

UCLASS()
class MAPLESTORY_API UInventoryManager : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	void Clear();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<int32, TObjectPtr<UItem>> EquipInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<int32, TObjectPtr<UItem>> UseInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<int32, TObjectPtr<UItem>> EtcInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<int32, TObjectPtr<UItem>> EquippedInventory;
};
