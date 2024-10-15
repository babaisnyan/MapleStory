#include "UI/InventoryWindow.h"

#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Data/Item.h"
#include "Managers/InventoryManager.h"
#include "UI/InventorySlot.h"

void UInventoryWindow::ShowEquip() {
	if (CurrentTabIndex == 0) {
		return;
	}

	CurrentTabIndex = 0;
	RefreshEquip();
}

void UInventoryWindow::ShowUse() {
	if (CurrentTabIndex == 1) {
		return;
	}

	CurrentTabIndex = 1;
	RefreshUse();
}

void UInventoryWindow::ShowEtc() {
	if (CurrentTabIndex == 2) {
		return;
	}

	CurrentTabIndex = 2;
	RefreshEtc();
}

void UInventoryWindow::RefreshEquip() {
	if (CurrentTabIndex != 0) {
		return;
	}

	const UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}


	WrapBox->ClearChildren();

	for (int i = 0; i < 128; i++) {
		UInventorySlot* KeyWidget = CreateWidget<UInventorySlot>(GetGameInstance(), QuickSlotKeyWidgetClass);
		KeyWidget->Type = CurrentTabIndex;
		KeyWidget->Pos = i;

		if (InventoryManager->EquipInventory.Contains(i)) {
			KeyWidget->ItemId = InventoryManager->EquipInventory[i]->Template.ItemId;
			KeyWidget->ItemCount = InventoryManager->EquipInventory[i]->Quantity;
		}

		WrapBox->AddChild(KeyWidget);
	}

	ScrollBox->ScrollToStart();
}

void UInventoryWindow::RefreshUse() {
	if (CurrentTabIndex != 1) {
		return;
	}

	const UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}


	WrapBox->ClearChildren();

	for (int i = 0; i < 128; i++) {
		UInventorySlot* KeyWidget = CreateWidget<UInventorySlot>(GetGameInstance(), QuickSlotKeyWidgetClass);
		KeyWidget->Type = CurrentTabIndex;
		KeyWidget->Pos = i;

		if (InventoryManager->UseInventory.Contains(i)) {
			KeyWidget->ItemId = InventoryManager->UseInventory[i]->Template.ItemId;
			KeyWidget->ItemCount = InventoryManager->UseInventory[i]->Quantity;
		}

		WrapBox->AddChild(KeyWidget);
	}

	ScrollBox->ScrollToStart();
}

void UInventoryWindow::RefreshEtc() {
	if (CurrentTabIndex != 2) {
		return;
	}

	const UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}


	WrapBox->ClearChildren();

	for (int i = 0; i < 128; i++) {
		UInventorySlot* KeyWidget = CreateWidget<UInventorySlot>(GetGameInstance(), QuickSlotKeyWidgetClass);
		KeyWidget->Type = CurrentTabIndex;
		KeyWidget->Pos = i;

		if (InventoryManager->EtcInventory.Contains(i)) {
			KeyWidget->ItemId = InventoryManager->EtcInventory[i]->Template.ItemId;
			KeyWidget->ItemCount = InventoryManager->EtcInventory[i]->Quantity;
		}

		WrapBox->AddChild(KeyWidget);
	}

	ScrollBox->ScrollToStart();
}
