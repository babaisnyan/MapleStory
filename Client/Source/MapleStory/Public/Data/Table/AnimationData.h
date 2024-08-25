#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "AnimationData.generated.h"

USTRUCT(BlueprintType)
struct MAPLESTORY_API FAnimationData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TSoftObjectPtr<UPaperSprite> Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	int32 Delay = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	int32 OffsetX = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	int32 OffsetY = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	int32 ZOrder = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bZigZag = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	bool HasAlpha = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	int32 AlphaStart = 255;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	int32 AlphaEnd = 0;
};
