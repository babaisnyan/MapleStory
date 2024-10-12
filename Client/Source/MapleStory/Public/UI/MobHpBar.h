#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobHpBar.generated.h"

class UProgressBar;

UCLASS()
class MAPLESTORY_API UMobHpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateHpBar() const;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MobHpBar")
	int32 MaxHp = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MobHpBar")
	int32 CurrentHp = 0;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HpBar;
	
};
