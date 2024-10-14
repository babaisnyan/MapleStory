#include "UI/InventorySlot.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/MsCursor.h"

bool UInventorySlot::Initialize() {
	LoadItemTexture();
	return Super::Initialize();
}

void UInventorySlot::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LoadItemTexture();
}

void UInventorySlot::LoadItemTexture() {
	if (!DummyButton || !ItemImage || !ItemCountText) {
		return;
	}

	if (!bInitialized) {
		DummyButton->OnClicked.AddDynamic(this, &UInventorySlot::OnClicked);
		bInitialized = true;
	}

	if (ItemId == 0) {
		DummyButton->SetCursor(EMouseCursor::Default);
		ItemImage->SetBrushFromTexture(nullptr, false);
		ItemImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
		return;
	}
	
	DummyButton->SetCursor(EMouseCursor::GrabHand);

	if (ItemCount == 0) {
		ItemCountText->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
	} else {
		ItemCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ItemCount)));
	}

	const FString Path = FString::Printf(TEXT("Texture2D'/Game/Item/Texture/T_%d_icon.T_%d_icon'"), ItemId, ItemId);
	ItemTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));

	if (ItemTexture) {
		FSlateBrush Brush;
		Brush.SetResourceObject(ItemTexture);
		Brush.DrawAs = ESlateBrushDrawType::Image;
		ItemImage->SetBrush(Brush);
		ItemImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UInventorySlot::OnClicked() {
	TArray<UUserWidget*> Cursors;
	UMsCursor* TempCursor = nullptr;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Cursors, UMsCursor::StaticClass(), false);


	if (ItemId == 0) {
		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);
			MsCursor->ClearImage();

			if (TempCursor == nullptr) {
				TempCursor = MsCursor;
			}
		}

		if (TempCursor && Type == TempCursor->ItemType) {
			ItemId = TempCursor->ItemId;
			ItemCount = TempCursor->ItemCount;
			UE_LOG(LogTemp, Warning, TEXT("ItemId: %d, SrcPos: %d, DestPos: %d"), ItemId, TempCursor->ItemPos, Pos);

			LoadItemTexture();
			SetCursor(EMouseCursor::GrabHand);
			// SEND_PACKET(FPacketCreator::GetChangeKeySetting(KeyCode, KeyType, ItemId, SkillId));

			if (TempCursor->PrevKeyWidget) {
				// SEND_PACKET(FPacketCreator::GetChangeKeySetting(TempCursor->PrevKeyWidget->KeyCode, EKeyType::None, 0, 0));
			}
		}

		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);
			MsCursor->Clear();
		}
	} else {
		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);

			if (TempCursor == nullptr) {
				TempCursor = MsCursor;
				break;
			}
		}

		if (TempCursor) {
			if (TempCursor->PrevInventorySlot) {} else {
				for (const auto Object : Cursors) {
					UMsCursor* MsCursor = Cast<UMsCursor>(Object);

					MsCursor->Attach(ItemTexture);
					MsCursor->ItemId = ItemId;
					MsCursor->ItemCount = ItemCount;
					MsCursor->ItemType = Type;
					MsCursor->ItemPos = Pos;
				}

				SetCursor(EMouseCursor::Default);
				TempCursor->PrevInventorySlot = this;

				ItemId = 0;
				ItemCount = 0;

				LoadItemTexture();
			}
		}
	}
}
