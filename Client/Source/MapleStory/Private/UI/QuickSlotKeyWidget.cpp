#include "UI/QuickSlotKeyWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/MsCursor.h"

bool UQuickSlotKeyWidget::Initialize() {
	LoadKeyCodeTexture();
	LoadKeyTexture();
	return Super::Initialize();
}

void UQuickSlotKeyWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LoadKeyCodeTexture();
	LoadKeyTexture();
}

void UQuickSlotKeyWidget::LoadKeyTexture() {
	if (DummyButton && !bInitialized) {
		DummyButton->OnClicked.AddDynamic(this, &UQuickSlotKeyWidget::OnClicked);
		bInitialized = true;
	}

	if (KeyImage) {
		KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (ItemCountText) {
		ItemCountText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	}

	if (KeyType == EKeyType::None) {
		if (KeyImage) {
			KeyImage->SetBrushFromTexture(nullptr, false);
			KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		}
		return;
	}

	if (KeyType == EKeyType::Item) {
		const FString Path = FString::Printf(TEXT("Texture2D'/Game/Item/Texture/T_%d_icon.T_%d_icon'"), ItemId, ItemId);
		KeyTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));

		if (KeyTexture && KeyImage) {
			KeyImage->SetBrushFromTexture(KeyTexture, false);
		}

		if (ItemCountText) {
			ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemCount)));
			ItemCountText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
	} else if (KeyType == EKeyType::Skill) {} else {
		if (!KeyTexturePaths.Contains(KeyType)) {
			return;
		}

		const FString& Path = KeyTexturePaths[KeyType];
		KeyTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));

		if (KeyTexture && KeyImage) {
			KeyImage->SetBrushFromTexture(KeyTexture, true);
		}
	}
}

void UQuickSlotKeyWidget::LoadKeyCodeTexture() {
	const int32_t KeyCodeValue = static_cast<int32_t>(KeyCode);
	const FString Path = FString::Printf(TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyConfig_%d.T_KeyConfig_%d'"), KeyCodeValue, KeyCodeValue);
	KeyCodeTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));

	if (KeyCodeTexture && KeyCodeImage) {
		KeyCodeImage->SetBrushFromTexture(KeyCodeTexture, true);
	}
}

void UQuickSlotKeyWidget::OnClicked() {
	if (KeyType == EKeyType::None) {
		return;
	}
	
	TArray<UUserWidget*> Cursors;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Cursors, UMsCursor::StaticClass(), false);
	
	for (const auto Object : Cursors) {
		UMsCursor* MsCursor = Cast<UMsCursor>(Object);
		MsCursor->Attach(KeyTexture);
	}
}
