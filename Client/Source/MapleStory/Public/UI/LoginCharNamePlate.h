
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginCharNamePlate.generated.h"

class UTextBlock;

UCLASS()
class MAPLESTORY_API ULoginCharNamePlate : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CharacterName;

	UPROPERTY(BlueprintReadWrite)
	bool IsSelected = false;
};
