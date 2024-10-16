
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Enum/EAvatarType.h"
#include "ChooseCharNameWindow.generated.h"

class UEditableTextBox;
class UPaperFlipbookUserWidget;

UCLASS()
class MAPLESTORY_API UChooseCharNameWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	EAvatarType Avatar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPaperFlipbookUserWidget> AvatarFlipBook;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> CharNameTextBox;
};
