#pragma once

#include "CoreMinimal.h"
#include "Data/Enum/ESoundEffectType.h"

#include "SoundManager.generated.h"

UCLASS()
class MAPLESTORY_API USoundManager : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
	explicit USoundManager();

	void PlaySoundEffect(ESoundEffectType Type, TObjectPtr<UWorld> World, bool bIsUISound = false) const;

private:
	TMap<ESoundEffectType, TObjectPtr<USoundBase>> SoundEffects;
};
