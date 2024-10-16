#pragma once

#include "CoreMinimal.h"
#include "NoticeWindow.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"


class UInventoryManager;
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class MAPLESTORY_API UInventorySlot : public UUserWidget {
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable)
	void LoadItemTexture();

private:
	UFUNCTION()
	void OnClicked();
	void OnRightClicked();
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Type = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Pos = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UTexture2D> ItemTexture;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCountText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> DummyButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UNoticeWindow> NoticeWindowClass;

private:
	bool bInitialized = false;
};
