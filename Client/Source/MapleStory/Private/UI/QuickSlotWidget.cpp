#include "UI/QuickSlotWidget.h"

#include "Blueprint/WidgetTree.h"
#include "UI/QuickSlotKeyWidget.h"

void UQuickSlotWidget::InitializeQuickSlotKeyWidgets() {
	WidgetTree->ForEachWidget([this](UWidget* Widget) {
		if (UQuickSlotKeyWidget* QuickSlotKeyWidget = Cast<UQuickSlotKeyWidget>(Widget)) {
			QuickSlotKeyWidgets.Add(QuickSlotKeyWidget->KeyCode, QuickSlotKeyWidget);
		}
	});

	for (const auto Pair : QuickSlotKeyWidgets) {
		const TObjectPtr<UQuickSlotKeyWidget> QuickSlotKeyWidget = Pair.Value;
		
		QuickSlotKeyWidget->SetVisibility(ESlateVisibility::Visible);
		QuickSlotKeyWidget->SetCursor(EMouseCursor::Type::Default);

		if (QuickSlotKeyWidget->KeyType != EKeyType::None) {
			QuickSlotKeyWidget->SetCursor(EMouseCursor::Type::GrabHand);
		}
	}
}
