#include "UI/QuickSlotKeyWidget.h"

#include "Components/Image.h"

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
	if (KeyImage) {
		KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (KeyType == EKeyType::None) {
		if (KeyImage) {
			KeyImage->SetBrushFromTexture(nullptr, false);
			KeyImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		}
		return;
	}

	if (KeyType == EKeyType::Item) {} else if (KeyType == EKeyType::Skill) {} else {
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
