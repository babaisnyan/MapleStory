#include "UI/QuickSlotWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Managers/InventoryManager.h"
#include "Managers/KeySettingManager.h"
#include "UI/QuickSlotKeyWidget.h"

void UQuickSlotWidget::Refresh() {
	for (const auto Pair : QuickSlotKeyWidgets) {
		const TObjectPtr<UQuickSlotKeyWidget> QuickSlotKeyWidget = Pair.Value;

		if (QuickSlotKeyWidget->KeyType == EKeyType::Item) {
			UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

			if (InventoryManager) {
				QuickSlotKeyWidget->ItemCount = InventoryManager->GetUseItemCount(QuickSlotKeyWidget->ItemId);
				QuickSlotKeyWidget->LoadKeyTexture();
			}
		}
	}
}

void UQuickSlotWidget::InitializeQuickSlotKeyWidgets() {
	const UKeySettingManager* KeySettingManager = GetGameInstance()->GetSubsystem<UKeySettingManager>();
	UInventoryManager* InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!KeySettingManager || !InventoryManager) {
		return;
	}

	WidgetTree->ForEachWidget([this](UWidget* Widget) {
		if (UQuickSlotKeyWidget* QuickSlotKeyWidget = Cast<UQuickSlotKeyWidget>(Widget)) {
			QuickSlotKeyWidgets.Add(QuickSlotKeyWidget->KeyCode, QuickSlotKeyWidget);
		}
	});

	for (const auto Pair : QuickSlotKeyWidgets) {
		const TObjectPtr<UQuickSlotKeyWidget> QuickSlotKeyWidget = Pair.Value;

		QuickSlotKeyWidget->SetVisibility(ESlateVisibility::Visible);
		QuickSlotKeyWidget->SetCursor(EMouseCursor::Type::Default);

		const auto KeySetting = KeySettingManager->Get(Pair.Key);

		if (KeySetting) {
			QuickSlotKeyWidget->KeyType = static_cast<EKeyType>(KeySetting->key_type());

			if (KeySetting->has_item_id()) {
				QuickSlotKeyWidget->ItemId = KeySetting->item_id();
				QuickSlotKeyWidget->ItemCount = InventoryManager->GetUseItemCount(KeySetting->item_id());
			} else if (KeySetting->has_skill_id()) {
				QuickSlotKeyWidget->SkillId = KeySetting->skill_id();
			}

			QuickSlotKeyWidget->LoadKeyTexture();
		}

		if (QuickSlotKeyWidget->KeyType != EKeyType::None) {
			QuickSlotKeyWidget->SetCursor(EMouseCursor::Type::GrabHand);
		}
	}
}
