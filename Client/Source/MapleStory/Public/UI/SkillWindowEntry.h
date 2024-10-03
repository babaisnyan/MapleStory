#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWindowEntry.generated.h"

class UImage;
class UTextBlock;
class UButton;

UCLASS()
class MAPLESTORY_API USkillWindowEntry : public UUserWidget {
	GENERATED_BODY()

private:
	void Setup(int32 Id, const FString& Name, int32 Level);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UTexture2D> SkillIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Icon")
	TObjectPtr<UTexture2D> SkillIconHover;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillLevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UpButton;

private:
	int32 SkillId = 0;
	int32 SkillLevel = 0;
	FString SkillName;
};
