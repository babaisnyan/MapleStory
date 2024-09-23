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
	UPaperSprite* GetMobMiss() const;
	UPaperSprite* GetPlayerMiss() const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	void LoadNumberTextures();

private:
	UPROPERTY()
	TArray<UPaperSprite*> MobNumberSprites;

	UPROPERTY()
	TArray<UPaperSprite*> PlayerNumberSprites;

	UPROPERTY()
	TArray<UPaperSprite*> CriticalPlayerNumberSprites;

	UPROPERTY()
	UPaperSprite* MobMissSprite;

	UPROPERTY()
	UPaperSprite* PlayerMissSprite;
};
