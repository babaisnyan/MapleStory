#include "Managers/InventoryManager.h"

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
