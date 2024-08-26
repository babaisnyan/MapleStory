#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "MsSpriteComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMsSpriteFinishedPlaySignature);

UCLASS(ShowCategories=(Mobility), meta=(BlueprintSpawnableComponent))
class MAPLESTORY_API UMsSpriteComponent : public UPaperSpriteComponent {
	GENERATED_BODY()

public:
	UMsSpriteComponent();
	
	void Setup(const UDataTable* SpriteTable, bool bAutoStart = true, bool bLoop = true);

	UFUNCTION(BlueprintCallable)
	void Reset();

    UFUNCTION(BlueprintCallable)
	void Play();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<bool> HasAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<int32> AlphaStarts;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TArray<int32> AlphaEnds;

public:
	UPROPERTY(BlueprintAssignable)
	FMsSpriteFinishedPlaySignature OnFinishedPlaying;

private:
	UPROPERTY()
	int32 CurrentIndex = 0;

	UPROPERTY()
	float TimeElapsed = 0.0f;

	UPROPERTY()
	FVector BaseOffset;

	UPROPERTY()
	bool bLooping = true;

	UPROPERTY()
	bool bEnded = false;

	UPROPERTY()
	bool bStarted = true;
};
