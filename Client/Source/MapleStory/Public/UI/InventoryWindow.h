#pragma once

#include "CoreMinimal.h"
#include "QuickSlotKeyWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWindow.generated.h"

class UWrapBox;
class UButton;
class UScrollBox;
class UTextBlock;

UCLASS()
class MAPLESTORY_API UInventoryWindow : public UUserWidget {
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void ShowEquip();

	UFUNCTION(BlueprintCallable)
	void ShowUse();

	UFUNCTION(BlueprintCallable)
	void ShowEtc();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UQuickSlotKeyWidget> QuickSlotKeyWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquipButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EtcButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MesoButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MesoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WrapBox;

private:
	int32 CurrentTabIndex = 0;
};
