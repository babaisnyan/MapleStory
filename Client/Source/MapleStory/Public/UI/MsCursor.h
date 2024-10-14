#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Enum/EKeyType.h"
#include "MsCursor.generated.h"

class UInventorySlot;
class UQuickSlotKeyWidget;
class UImage;
class UPaperFlipbookUserWidget;
class UPaperFlipbook;

UCLASS()
class MAPLESTORY_API UMsCursor : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ClearImage();

	UFUNCTION(BlueprintCallable)
	void Attach(UTexture2D* InSprite);

	void Clear();
	
protected:
	UFUNCTION(BlueprintCallable)
	void ApplyCursorFlipbook() const;

public:
	UPROPERTY()
	EKeyType KeyType = EKeyType::None;

	UPROPERTY()
	int32 ItemId = 0;

	UPROPERTY()
	int32 ItemType = 0;

	UPROPERTY()
	int32 ItemPos = 0;
	
	UPROPERTY()
	int32 ItemCount = 0;
	
	UPROPERTY()
	int32 SkillId = 0;

	UPROPERTY()
	TObjectPtr<UQuickSlotKeyWidget> PrevKeyWidget;

	UPROPERTY()
	TObjectPtr<UInventorySlot> PrevInventorySlot;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPaperFlipbookUserWidget> CursorImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> AttachImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UTexture2D> AttachTexture;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> CursorFlipbook;
};
