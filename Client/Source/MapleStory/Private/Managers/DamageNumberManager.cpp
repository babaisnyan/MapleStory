#include "Managers/DamageNumberManager.h"

#include "PaperSprite.h"

void UDamageNumberManager::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	LoadNumberTextures();
}

void UDamageNumberManager::LoadNumberTextures() {
	MobNumberSprites.Empty();
	PlayerNumberSprites.Empty();
	CriticalPlayerNumberSprites.Empty();

	for (int32 i = 0; i < 10; i++) {
		{
			FString TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_MobDamage_%d.S_MobDamage_%d"), i, i);
			UPaperSprite* Sprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);

			if (Sprite) {
				MobNumberSprites.Add(Sprite);
			}
		}

		{
			FString TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_PlayerDamage_%d.S_PlayerDamage_%d"), i, i);
			UPaperSprite* Sprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);

			if (Sprite) {
				PlayerNumberSprites.Add(Sprite);
			}
		}

		{
			FString TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_PlayerDamage_Crit_%d.S_PlayerDamage_Crit_%d"), i, i);
			UPaperSprite* Sprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);

			if (Sprite) {
				CriticalPlayerNumberSprites.Add(Sprite);
			}
		}
	}

	FString TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_MobDamage_Miss.S_MobDamage_Miss"));
	MobMissSprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);
	TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_PlayerDamage_Miss.S_PlayerDamage_Miss"));
	PlayerMissSprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);
}

UPaperSprite* UDamageNumberManager::GetMobNumber(const int32 Digit) {
	check(MobNumberSprites.IsValidIndex(Digit));
	return MobNumberSprites[Digit];
}

UPaperSprite* UDamageNumberManager::GetPlayerNumber(const int32 Digit) {
	check(PlayerNumberSprites.IsValidIndex(Digit));
	return PlayerNumberSprites[Digit];
}

UPaperSprite* UDamageNumberManager::GetCriticalPlayerNumber(const int32 Digit) {
	check(CriticalPlayerNumberSprites.IsValidIndex(Digit));
	return CriticalPlayerNumberSprites[Digit];
}

UPaperSprite* UDamageNumberManager::GetMobMiss() const {
	check(MobMissSprite);
	return MobMissSprite;
}

UPaperSprite* UDamageNumberManager::GetPlayerMiss() const {
	check(PlayerMissSprite);
	return PlayerMissSprite;
}
