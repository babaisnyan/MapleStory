#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "MsSpriteComponent.generated.h"

UCLASS(ShowCategories=(Mobility), meta=(BlueprintSpawnableComponent))
class MAPLESTORY_API UMsSpriteComponent : public UPaperSpriteComponent {
	GENERATED_BODY()

public:
	void Setup(const UDataTable* SpriteTable, bool bZigZag = false);
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<TObjectPtr<UPaperSprite>> Sprites;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<float> Delays;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<FVector2D> Offsets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<int32> ZOrders;
	
private:
	UPROPERTY()
	int32 CurrentIndex = 0;

	UPROPERTY()
	float TimeElapsed = 0.0f;

	UPROPERTY()
	FVector BaseOffset;
};
