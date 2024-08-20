#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MsCursor.generated.h"

class UImage;
class UPaperFlipbookUserWidget;
class UPaperFlipbook;

UCLASS()
class MAPLESTORY_API UMsCursor : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ClearImage();

	UFUNCTION(BlueprintCallable)
	void Attach(UTexture2D* InSprite);
	
protected:
	UFUNCTION(BlueprintCallable)
	void ApplyCursorFlipbook() const;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UPaperFlipbookUserWidget> CursorImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> AttachImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UTexture2D> AttachTexture;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbook> CursorFlipbook;
};
