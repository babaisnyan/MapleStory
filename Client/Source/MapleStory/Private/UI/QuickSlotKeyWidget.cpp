#include "UI/QuickSlotKeyWidget.h"

#include "MapleGameInstance.h"
#include "MapleStory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Managers/KeySettingManager.h"
#include "Network/PacketCreator.h"
#include "UI/InventorySlot.h"
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

		if (ItemCountText && bShowCount) {
			ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemCount)));
			ItemCountText->SetColorAndOpacity(CountColor);
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
	TArray<UUserWidget*> Cursors;
	UMsCursor* TempCursor = nullptr;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Cursors, UMsCursor::StaticClass(), false);

	if (KeyType == EKeyType::None) {
		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);
			MsCursor->ClearImage();

			if (TempCursor == nullptr) {
				TempCursor = MsCursor;
			}
		}

		if (TempCursor) {
			if (TempCursor->PrevInventorySlot) {
				OnAssignItem(TempCursor);
				return;
			}

			KeyType = TempCursor->KeyType;
			ItemId = TempCursor->ItemId;
			ItemCount = TempCursor->ItemCount;
			SkillId = TempCursor->SkillId;

			LoadKeyTexture();
			SetCursor(EMouseCursor::GrabHand);
			SEND_PACKET(FPacketCreator::GetChangeKeySetting(KeyCode, KeyType, ItemId, SkillId));

			if (TempCursor->PrevKeyWidget) {
				SEND_PACKET(FPacketCreator::GetChangeKeySetting(TempCursor->PrevKeyWidget->KeyCode, EKeyType::None, 0, 0));
			}
		}

		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);
			MsCursor->Clear();
		}
	} else if (KeyType == EKeyType::Item) {
		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);

			if (TempCursor == nullptr) {
				TempCursor = MsCursor;
				break;
			}
		}

		if (TempCursor) {
			if (TempCursor->PrevKeyWidget) {
				SetCursor(EMouseCursor::GrabHand);

				Swap(KeyType, TempCursor->KeyType);
				Swap(ItemId, TempCursor->ItemId);
				Swap(ItemCount, TempCursor->ItemCount);
				Swap(SkillId, TempCursor->SkillId);
				SEND_PACKET(FPacketCreator::GetChangeKeySetting(KeyCode, KeyType, ItemId, SkillId));
				UE_LOG(LogTemp, Warning, TEXT("KeyCode: %d, KeyType: %d, ItemId: %d, SkillId: %d"), KeyCode, KeyType, ItemId, SkillId);

				TempCursor->PrevKeyWidget->KeyType = TempCursor->KeyType;
				TempCursor->PrevKeyWidget->ItemId = TempCursor->ItemId;
				TempCursor->PrevKeyWidget->ItemCount = TempCursor->ItemCount;
				TempCursor->PrevKeyWidget->SkillId = TempCursor->SkillId;
				SEND_PACKET(FPacketCreator::GetChangeKeySetting(TempCursor->PrevKeyWidget->KeyCode, TempCursor->PrevKeyWidget->KeyType, TempCursor->PrevKeyWidget->ItemId, TempCursor->PrevKeyWidget->SkillId));
				UE_LOG(LogTemp, Warning, TEXT("KeyCode: %d, KeyType: %d, ItemId: %d, SkillId: %d"), TempCursor->PrevKeyWidget->KeyCode, TempCursor->PrevKeyWidget->KeyType, TempCursor->PrevKeyWidget->ItemId, TempCursor->PrevKeyWidget->SkillId);

				LoadKeyTexture();
				TempCursor->PrevKeyWidget->LoadKeyTexture();

				for (const auto Object : Cursors) {
					UMsCursor* MsCursor = Cast<UMsCursor>(Object);
					MsCursor->ClearImage();
					MsCursor->Clear();
				}
			} else {
				for (const auto Object : Cursors) {
					UMsCursor* MsCursor = Cast<UMsCursor>(Object);

					MsCursor->Attach(KeyTexture);
					MsCursor->KeyType = KeyType;
					MsCursor->ItemId = ItemId;
					MsCursor->ItemCount = ItemCount;
					MsCursor->SkillId = SkillId;
				}

				SetCursor(EMouseCursor::Default);
				TempCursor->PrevKeyWidget = this;

				KeyType = EKeyType::None;
				ItemId = 0;
				ItemCount = 0;
				SkillId = 0;

				LoadKeyTexture();
			}
		}
	}
}

void UQuickSlotKeyWidget::OnAssignItem(UMsCursor* MsCursor) {
	ItemId = MsCursor->ItemId;
	ItemCount = MsCursor->ItemCount;
	KeyType = EKeyType::Item;

	MsCursor->PrevInventorySlot->ItemId = ItemId;
	MsCursor->PrevInventorySlot->ItemCount = ItemCount;
	MsCursor->PrevInventorySlot->LoadItemTexture();
	MsCursor->PrevInventorySlot->SetCursor(EMouseCursor::GrabHand);

	MsCursor->Clear();

	LoadKeyTexture();
	SetCursor(EMouseCursor::GrabHand);

	SEND_PACKET(FPacketCreator::GetChangeKeySetting(KeyCode, KeyType, ItemId, SkillId));
}

void UQuickSlotKeyWidget::OnRightClicked() {
	const TObjectPtr<UKeySettingManager> KeySettingManager = GetGameInstance()->GetSubsystem<UKeySettingManager>();

	if (!KeySettingManager) {
		return;
	}

	KeyType = EKeyType::None;
	ItemId = 0;
	ItemCount = 0;

	SEND_PACKET(FPacketCreator::GetChangeKeySetting(KeyCode, KeyType, ItemId, SkillId));
}

FReply UQuickSlotKeyWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton) {
		return FReply::Handled();
	}

	if (KeyType != EKeyType::Item) {
		return FReply::Handled();
	}

	if (!GetGameInstance()) {
		return FReply::Handled();
	}

	AsyncTask(ENamedThreads::GameThread, [this] {
		if (GetGameInstance()) {
			OnRightClicked();
		}
	});

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
