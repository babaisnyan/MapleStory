#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWindow.generated.h"

class UScrollBox;
class UTextBlock;

UCLASS()
class MAPLESTORY_API USkillWindow : public UUserWidget {
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillPoint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;
};
