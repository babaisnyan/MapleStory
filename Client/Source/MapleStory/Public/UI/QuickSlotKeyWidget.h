#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Enum/EKeyType.h"
#include "QuickSlotKeyWidget.generated.h"

class UImage;

UCLASS()
class MAPLESTORY_API UQuickSlotKeyWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UTexture2D> LoadKeyTexture();

public:
	UPROPERTY(BlueprintReadWrite)
	EKeyType KeyType = EKeyType::None;

	UPROPERTY(BlueprintReadWrite)
	int32 ItemId = 0;

	UPROPERTY(BlueprintReadWrite)
	int32 SkillId = 0;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> KeyCodeImage;
};
