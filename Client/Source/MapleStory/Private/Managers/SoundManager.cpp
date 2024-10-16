#include "Managers/SoundManager.h"

#include "Kismet/GameplayStatics.h"


USoundManager::USoundManager() {
	#if WITH_EDITOR
	const TObjectPtr<UEnum> SoundEffectEnum = StaticEnum<ESoundEffectType>();

	if (SoundEffectEnum) {
		for (int32 i = 0; i < SoundEffectEnum->NumEnums() - 1; i++) {
			FString Path = SoundEffectEnum->GetMetaData(TEXT("Path"), i);
			TObjectPtr<USoundBase> Sound = LoadObject<USoundBase>(nullptr, *Path);

			if (Sound) {
				ESoundEffectType Type = static_cast<ESoundEffectType>(i);
				SoundEffects.Emplace(Type, Sound);
			}
		}
	}
	#endif
}


void USoundManager::PlaySoundEffect(const ESoundEffectType Type, const TObjectPtr<UWorld>& World, const bool bIsUISound) const {
	if (SoundEffects.Contains(Type)) {
		UGameplayStatics::PlaySound2D(World.Get(), SoundEffects[Type].Get(), 1.5f, 1.0f, 0.0f, nullptr, nullptr, bIsUISound);
	}
}

void USoundManager::PlaySoundEffect(const TSoftObjectPtr<USoundWave>& Sound) const {
	USoundWave* Asset = Sound.LoadSynchronous();

	if (Asset) {
		UGameplayStatics::PlaySound2D(GetWorld(), Asset, 1.0f, 1.0f, 0.0f, nullptr, nullptr, true);
	}
}
