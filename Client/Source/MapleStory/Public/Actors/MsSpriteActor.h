#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MsSpriteActor.generated.h"

class UMsSpriteComponent;

UCLASS()
class MAPLESTORY_API AMsSpriteActor : public AActor {
	GENERATED_BODY()

public:
	AMsSpriteActor();

	UFUNCTION(BlueprintCallable)
	void Setup(const UDataTable* SpriteTable, int32 StartDelay = 0);

	UFUNCTION(BlueprintCallable)
	void SetPriority(int32 Priority) const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnDelayFinished();

	UFUNCTION()
	void OnFinished(UMsSpriteComponent* Discard);

private:
	UPROPERTY()
	TObjectPtr<UMsSpriteComponent> SpriteComponent;

	float Delay = 0;
	FTimerHandle TimerHandle;
};
