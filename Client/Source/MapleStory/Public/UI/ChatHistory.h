#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatHistory.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class MAPLESTORY_API UChatHistory : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetText(const FString& InText) const;
	void SetText(const FString& InText, const FColor& BackColor, const FColor& TextColor) const;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Back;
};
