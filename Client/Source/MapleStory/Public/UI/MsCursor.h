#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MsCursor.generated.h"


class UPaperFlipbookUserWidget;
class UPaperFlipbook;

UCLASS()
class MAPLESTORY_API UMsCursor : public UUserWidget {
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	void ApplyCursorFlipbook() const;
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPaperFlipbookUserWidget> CursorImage;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> CursorFlipbook;
};
