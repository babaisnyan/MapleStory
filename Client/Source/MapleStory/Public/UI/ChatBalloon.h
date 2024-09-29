#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatBalloon.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class MAPLESTORY_API UChatBalloon : public UUserWidget {
	GENERATED_BODY()

public:
	void SetText(const FString& InText) const;

protected:
	virtual void NativePreConstruct() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Text")
	FSlateColor Color = FColor::White;
	
	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> Arrow;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> Center;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> Left;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> Right;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> Top;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> Bottom;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> TopLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> TopRight;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> BottomLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Image")
	TObjectPtr<UTexture2D> BottomRight;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> TopLeftImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> TopImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> TopRightImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LeftImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CenterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> RightImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BottomLeftImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BottomImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BottomRightImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ArrowImage;
};
