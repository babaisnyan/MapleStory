#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "Blueprint/UserWidget.h"
#include "EquipWindow.generated.h"

UCLASS()
class MAPLESTORY_API UEquipWindow : public UUserWidget {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void LoadIcons();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInventorySlot> ItemWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TMap<int32, TObjectPtr<UInventorySlot>> ItemWidgets;
};
