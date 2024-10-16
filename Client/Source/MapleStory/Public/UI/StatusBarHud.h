
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarHud.generated.h"

class URichTextBlock;
class UTextBlock;
class UImage;

UCLASS()
class MAPLESTORY_API UStatusBarHud : public UUserWidget {
	GENERATED_BODY()

public:
	void UpdateGauge(int32 Hp, int32 MaxHp, int32 Mp, int32 MaxMp, int32 Exp, int32 MaxExp) const;
	void UpdateLevel(int32 Level) const;
	void SetName(const FString& Name) const;
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> HpImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> MpImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ExpImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> HpText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MpText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> ExpText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> NameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<URichTextBlock> LevelText;
};
