#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTextActor.generated.h"

class UDamageNumberWidget;
class UWidgetComponent;

UCLASS()
class MAPLESTORY_API ADamageTextActor : public AActor {
	GENERATED_BODY()

public:
	ADamageTextActor();
	void SetDamage(int32 Damage, bool bMob, bool bCritical);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnFadeOutFinished();

private:
	UPROPERTY()
	TObjectPtr<UWidgetComponent> DamageWidgetComponent;

	UPROPERTY()
	TSubclassOf<UDamageNumberWidget> DamageWidgetClass;

	bool bIsMob;
	bool bIsCritical;
	int32 DamageValue;
};
