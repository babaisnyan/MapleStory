#include "UI/EquipWindow.h"

#include "Data/Item.h"
#include "Managers/InventoryManager.h"

void UEquipWindow::LoadIcons() {
	if (ItemWidgets.Num() != 13) {
		return;
	}
	
	const TObjectPtr<UInventoryManager> InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}

	for (const auto& Equipped : InventoryManager->EquippedInventory) {
		const int32 ItemId = Equipped.Value->Template.ItemId;
		ItemWidgets[Equipped.Key]->ItemId = ItemId;
		ItemWidgets[Equipped.Key]->LoadItemTexture();
	}
}
