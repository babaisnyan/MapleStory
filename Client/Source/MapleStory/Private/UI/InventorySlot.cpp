#include "UI/InventorySlot.h"

#include "MapleGameInstance.h"
#include "MapleStory.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Managers/InventoryManager.h"
#include "Network/PacketCreator.h"
#include "UI/EquipWindow.h"
#include "UI/InventoryWindow.h"
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

	if (ItemCount == 0 || Type == 0 || Type == 3) {
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
	if (Type == 3) {
		return;
	}

	TArray<UUserWidget*> Cursors;
	UMsCursor* TempCursor = nullptr;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Cursors, UMsCursor::StaticClass(), false);
	const TObjectPtr<UInventoryManager> InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (ItemId == 0) {
		for (const auto Object : Cursors) {
			UMsCursor* MsCursor = Cast<UMsCursor>(Object);
			MsCursor->ClearImage();

			if (TempCursor == nullptr) {
				TempCursor = MsCursor;
			}
		}

		if (TempCursor && Type == TempCursor->ItemType) {
			if (TempCursor->PrevInventorySlot) {
				ItemId = TempCursor->ItemId;
				ItemCount = TempCursor->ItemCount;
				LoadItemTexture();
				SetCursor(EMouseCursor::GrabHand);

				if (Pos == TempCursor->ItemPos) {
					SEND_PACKET(FPacketCreator::GetUseItemRequest(Pos));
					InventoryManager->UseItem(Pos);
				} else {
					SEND_PACKET(FPacketCreator::GetMoveItemRequest(Type, TempCursor->ItemPos, Pos));

					switch (Type) {
						case 0:
							InventoryManager->MoveItemEquip(TempCursor->ItemPos, Pos);
							break;
						case 1:
							InventoryManager->MoveItemUse(TempCursor->ItemPos, Pos);
							break;
						case 2:
							InventoryManager->MoveItemEtc(TempCursor->ItemPos, Pos);
							break;
					}
				}

				TempCursor->PrevInventorySlot = nullptr;
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
			if (TempCursor->PrevInventorySlot && TempCursor && Type == TempCursor->ItemType) {
				SetCursor(EMouseCursor::GrabHand);

				switch (Type) {
					case 0:
						InventoryManager->MoveItemEquip(TempCursor->ItemPos, Pos);
						break;
					case 1:
						InventoryManager->MoveItemUse(TempCursor->ItemPos, Pos);
						break;
					case 2:
						InventoryManager->MoveItemEtc(TempCursor->ItemPos, Pos);
						break;
				}

				SEND_PACKET(FPacketCreator::GetMoveItemRequest(Type, TempCursor->ItemPos, Pos));

				Swap(ItemId, TempCursor->ItemId);
				Swap(ItemCount, TempCursor->ItemCount);
				TempCursor->PrevInventorySlot->ItemId = TempCursor->ItemId;
				TempCursor->PrevInventorySlot->ItemCount = TempCursor->ItemCount;

				LoadItemTexture();
				TempCursor->PrevInventorySlot->LoadItemTexture();

				for (const auto Object : Cursors) {
					UMsCursor* MsCursor = Cast<UMsCursor>(Object);
					MsCursor->ClearImage();
					MsCursor->Clear();
				}
			} else {
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

void UInventorySlot::OnRightClicked() {
	const TObjectPtr<UInventoryManager> InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();

	if (!InventoryManager) {
		return;
	}

	if (Type == 0) {
		if (!InventoryManager->CanEquip(Pos)) {
			// TODO: 조건 안되서 못낌
			return;
		}

		const int32 EquipPos = InventoryManager->Equip(Pos);

		if (EquipPos == -1) {
			return;
		}

		SEND_PACKET(FPacketCreator::GetEquipItemRequest(Pos, EquipPos));
	} else if (Type == 3) {
		const int32 UnEquipPos = InventoryManager->UnEquip(Pos);

		if (UnEquipPos == -1) {
			return;
		}

		SEND_PACKET(FPacketCreator::GetUnEquipItemRequest(Pos, UnEquipPos));
	}

	TArray<UUserWidget*> Windows;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Windows, UInventoryWindow::StaticClass(), false);

	for (const auto Object : Windows) {
		UInventoryWindow* InventoryWindow = Cast<UInventoryWindow>(Object);

		if (InventoryWindow) {
			InventoryWindow->RefreshEquip();
		}
	}

	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Windows, UEquipWindow::StaticClass(), false);

	for (const auto Object : Windows) {
		UEquipWindow* EquipWindow = Cast<UEquipWindow>(Object);

		if (EquipWindow) {
			EquipWindow->LoadIcons();
		}
	}

	ItemId = 0;
	LoadItemTexture();
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	if (InMouseEvent.GetEffectingButton() != EKeys::RightMouseButton) {
		return FReply::Handled();
	}

	if (Type != 0 && Type != 3 && ItemId == 0) {
		return FReply::Handled();
	}

	AsyncTask(ENamedThreads::GameThread, [this] {
		if (GetGameInstance()) {
			OnRightClicked();
		}
	});

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}
