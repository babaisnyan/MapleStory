#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoticeWindow.generated.h"

class UTextBlock;

UCLASS()
class MAPLESTORY_API UNoticeWindow : public UUserWidget {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText Text;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NoticeText;
};
