#include "Managers/InventoryManager.h"

#include "MapleGameInstance.h"
#include "Characters/MsLocalPlayer.h"
#include "Components/PlayerStatComponent.h"
#include "Data/Item.h"
#include "Managers/SoundManager.h"

void UInventoryManager::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);

	if (!GetGameInstance()) {
		return;
	}

	SoundManager = GetGameInstance()->GetSubsystem<USoundManager>();
	UseSound = LoadObject<USoundWave>(nullptr, TEXT("/Game/Sound/Sfx/SFX_Use.SFX_Use"));
}

void UInventoryManager::Deinitialize() {
	Super::Deinitialize();
	Clear();
}

void UInventoryManager::Clear() {
	EquipInventory.Empty();
	UseInventory.Empty();
	EtcInventory.Empty();
	EquippedInventory.Empty();
}

void UInventoryManager::MoveItemEquip(const int32 From, const int32 To) {
	MoveItem(EquipInventory, From, To);
}

void UInventoryManager::MoveItemUse(const int32 From, const int32 To) {
	MoveItem(UseInventory, From, To);
}

void UInventoryManager::MoveItemEtc(const int32 From, const int32 To) {
	MoveItem(EtcInventory, From, To);
}

int32 UInventoryManager::Equip(const int32 From) {
	if (!EquipInventory.Contains(From)) {
		return -1;
	}

	const TObjectPtr<UItem>& Item = EquipInventory[From];
	const int32 Pos = GetEquipPos(Item->Template.SubType, Item->Template.ItemId);

	if (EquippedInventory.Contains(Pos)) {
		Swap(EquippedInventory[Pos], EquipInventory[From]);
		return Pos;
	}

	if (Pos == 0) {
		return -1;
	}

	EquippedInventory.Add(Pos, Item);
	EquipInventory.Remove(From);

	return Pos;
}

int32 UInventoryManager::UnEquip(const int32 From) {
	if (!EquippedInventory.Contains(From) || EquipInventory.Num() >= 128) {
		return -1;
	}

	const TObjectPtr<UItem>& Item = EquippedInventory[From];
	int32 Pos = 0;

	for (int32 i = 0; i < 128; i++) {
		if (EquipInventory.Contains(i)) {
			continue;
		}

		Pos = i;
		break;
	}

	EquipInventory.Add(Pos, Item);
	EquippedInventory.Remove(From);

	return Pos;
}

void UInventoryManager::UseItem(const int32 Pos) {
	if (!UseInventory.Contains(Pos)) {
		return;
	}

	const TObjectPtr<UItem>& Item = UseInventory[Pos];

	if (Item->Template.ItemId == 0) {
		return;
	}

	if (--Item->Quantity <= 0) {
		UseInventory.Remove(Pos);
	}

	if (SoundManager && UseSound) {
		SoundManager->PlaySoundEffect(UseSound);
	}
}

int32 UInventoryManager::UseItemById(const int32 ItemId) {
	if (GetUseItemCount(ItemId) < 1) {
		return -1;
	}

	for (const auto Pair : UseInventory) {
		const TObjectPtr<UItem>& Item = Pair.Value;

		if (Item->Template.ItemId == ItemId) {
			UseItem(Pair.Key);
			return Pair.Key;
		}
	}

	return -1;
}

int32 UInventoryManager::GetUseItemCount(const int32 ItemId) {
	int32 Count = 0;

	for (const auto Pair : UseInventory) {
		const TObjectPtr<UItem>& Item = Pair.Value;

		if (Item->Template.ItemId == ItemId) {
			Count += Item->Quantity;
		}
	}

	return Count;
}

bool UInventoryManager::CanEquip(const int32 Pos) {
	if (!EquipInventory.Contains(Pos)) {
		return false;
	}

	const UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());

	if (!GameInstance) {
		return false;
	}

	const TObjectPtr<UItem> Item = EquipInventory[Pos];
	const FItemTemplate& ItemInfo = Item->Template;
	const TObjectPtr<UPlayerStatComponent> PlayerStat = GameInstance->CurrentPlayer->PlayerStat;

	if (ItemInfo.ReqLevel > PlayerStat->Level) {
		return false;
	}

	if (ItemInfo.ReqStr > PlayerStat->Str) {
		return false;
	}

	if (ItemInfo.ReqDex > PlayerStat->Dex) {
		return false;
	}

	if (ItemInfo.ReqInt > PlayerStat->Int) {
		return false;
	}

	if (ItemInfo.ReqLuk > PlayerStat->Luk) {
		return false;
	}

	return true;
}

void UInventoryManager::MoveItem(TMap<int32, TObjectPtr<UItem>>& Inventory, const int32 From, const int32 To) {
	if (From == To) {
		return;
	}

	if (!Inventory.Contains(From)) {
		return;
	}

	if (!Inventory.Contains(To)) {
		Inventory.Add(To, Inventory[From]);
		Inventory.Remove(From);
	} else {
		const TObjectPtr<UItem>& FromItem = Inventory[From];
		const TObjectPtr<UItem>& ToItem = Inventory[To];

		if (FromItem->Template.ItemId != ToItem->Template.ItemId) {
			Swap(Inventory[From], Inventory[To]);
			return;
		}

		const int32 MaxSlot = FromItem->Template.SlotMax;

		if (FromItem->Quantity + ToItem->Quantity > MaxSlot) {
			const int32 Remain = FromItem->Quantity + ToItem->Quantity - MaxSlot;
			ToItem->Quantity = MaxSlot;
			FromItem->Quantity = Remain;
		} else {
			ToItem->Quantity += FromItem->Quantity;
			Inventory.Remove(From);
		}
	}
}

int32 UInventoryManager::GetEquipPos(const ESubItemType Type, const int32 ItemId) {
	switch (Type) {
		case ESubItemType::Cap:
			return 1;
		case ESubItemType::Coat:
			return 7;
		case ESubItemType::Glove:
			return 10;
		case ESubItemType::Pants:
			return 11;
		case ESubItemType::Ring:
			return 3;
		case ESubItemType::Shoes:
			return 13;
		case ESubItemType::Weapon:
			return 9;
	}

	const int32 Prefix = ItemId / 1000000;

	switch (Prefix) {
		case 1012:
			return 2;
		case 1022:
			return 4;
		case 1032:
			return 5;
		case 1122:
			return 8;
		case 1132:
			return 12;
		case 1152:
			return 6;
	}

	return 0;
}
