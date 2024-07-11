#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginMessageWindow.generated.h"

UCLASS()
class MAPLESTORY_API ULoginMessageWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	int32 ErrorCode;
};
