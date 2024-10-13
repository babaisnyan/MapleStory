#include "UI/InventoryWindow.h"

#include "Components/ScrollBox.h"
#include "Components/WrapBox.h"
#include "Data/Item.h"
#include "Managers/InventoryManager.h"

void UInventoryWindow::ShowEquip() {
	if (CurrentTabIndex == 0) {
		return;
	}

	const UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}

	CurrentTabIndex = 0;
	WrapBox->ClearChildren();

	ScrollBox->ScrollToStart();
}

void UInventoryWindow::ShowUse() {
	if (CurrentTabIndex == 1) {
		return;
	}

	const UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}

	CurrentTabIndex = 1;
	WrapBox->ClearChildren();

	for (int i = 0; i < 128; i++) {
		UQuickSlotKeyWidget* QuickSlotKeyWidget = CreateWidget<UQuickSlotKeyWidget>(GetGameInstance(), QuickSlotKeyWidgetClass);
		QuickSlotKeyWidget->CountColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
		QuickSlotKeyWidget->KeyType = EKeyType::Item;
		QuickSlotKeyWidget->KeyCode = EKeyCode::None;
		QuickSlotKeyWidget->bShowCount = false;
		QuickSlotKeyWidget->SetRenderScale(FVector2D(1.0f, 1.0f));

		if (InventoryManager->UseInventory.Contains(i)) {
			QuickSlotKeyWidget->ItemId = InventoryManager->UseInventory[i]->Template.ItemId;
			QuickSlotKeyWidget->ItemCount = InventoryManager->UseInventory[i]->Quantity;
			QuickSlotKeyWidget->bShowCount = true;
		} else {
			QuickSlotKeyWidget->SetIsEnabled(false);
		}

		WrapBox->AddChild(QuickSlotKeyWidget);
	}

	ScrollBox->ScrollToStart();
}

void UInventoryWindow::ShowEtc() {
	if (CurrentTabIndex == 2) {
		return;
	}

	const UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}

	CurrentTabIndex = 2;
	WrapBox->ClearChildren();

	ScrollBox->ScrollToStart();
}
