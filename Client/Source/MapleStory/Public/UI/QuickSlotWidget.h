#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Enum/EKeyCode.h"
#include "QuickSlotWidget.generated.h"

class UQuickSlotKeyWidget;

UCLASS()
class MAPLESTORY_API UQuickSlotWidget : public UUserWidget {
	GENERATED_BODY()

public:
	void Refresh();
	
protected:
	UFUNCTION(BlueprintCallable)
	void InitializeQuickSlotKeyWidgets();

private:
	UPROPERTY()
	TMap<EKeyCode, TObjectPtr<UQuickSlotKeyWidget>> QuickSlotKeyWidgets;
};
