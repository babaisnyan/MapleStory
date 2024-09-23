#pragma once

#include "CoreMinimal.h"
#include "DamageNumberManager.generated.h"

class UPaperSprite;

UCLASS()
class MAPLESTORY_API UDamageNumberManager : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	UPaperSprite* GetMobNumber(int32 Digit);
	UPaperSprite* GetPlayerNumber(int32 Digit);
	UPaperSprite* GetCriticalPlayerNumber(int32 Digit);
	UTexture2D* GetMobMiss() const;
	UTexture2D* GetPlayerMiss() const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void LoadNumberTextures();

private:
	UPROPERTY()
	TArray<TObjectPtr<UPaperSprite>> MobNumberSprites;

	UPROPERTY()
	TArray<TObjectPtr<UPaperSprite>> PlayerNumberSprites;

	UPROPERTY()
	TArray<TObjectPtr<UPaperSprite>> CriticalPlayerNumberSprites;

	UPROPERTY()
	TObjectPtr<UTexture2D> MobMissTexture;

	UPROPERTY()
	TObjectPtr<UTexture2D> PlayerMissTexture;
};
