

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobNameTag.generated.h"

class UTextBlock;

UCLASS()
class MAPLESTORY_API UMobNameTag : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetInfo(int32 Level, const FName& Name) const;
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> MobName;
};
