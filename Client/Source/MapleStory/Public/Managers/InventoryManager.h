#pragma once

#include "CoreMinimal.h"
#include "Data/Enum/ESubItemType.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventoryManager.generated.h"

class UItem;

UCLASS()
class MAPLESTORY_API UInventoryManager : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	void Clear();

	void MoveItemEquip(int32 From, int32 To);
	void MoveItemUse(int32 From, int32 To);
	void MoveItemEtc(int32 From, int32 To);
	int32 Equip(int32 From);
	int32 UnEquip(int32 From);
	void UseItem(int32 Pos);
	void UseItemById(int32 ItemId);
	int32 GetUseItemCount(int32 ItemId);
	bool CanEquip(int32 Pos);

private:
	void MoveItem(TMap<int32, TObjectPtr<UItem>>& Inventory, int32 From, int32 To);
	int32 GetEquipPos(ESubItemType Type, int32 ItemId);

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
