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
		FString TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_MobDamage_%d.S_MobDamage_%d"), i, i);
		UPaperSprite* Sprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);

		if (Sprite) {
			MobNumberSprites.Add(Sprite);
		}


		TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_PlayerDamage_%d.S_PlayerDamage_%d"), i, i);
		Sprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);

		if (Sprite) {
			PlayerNumberSprites.Add(Sprite);
		}


		TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/S_PlayerDamage_Crit_%d.S_PlayerDamage_Crit_%d"), i, i);
		Sprite = LoadObject<UPaperSprite>(nullptr, *TexturePath);

		if (Sprite) {
			CriticalPlayerNumberSprites.Add(Sprite);
		}
	}

	FString TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/T_MobDamage_Miss.T_MobDamage_Miss"));
	MobMissTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
	TexturePath = FString::Printf(TEXT("/Game/Misc/Damage/T_PlayerDamage_Miss.T_PlayerDamage_Miss"));
	PlayerMissTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);
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

UTexture2D* UDamageNumberManager::GetMobMiss() const {
	check(MobMissTexture);
	return MobMissTexture;
}

UTexture2D* UDamageNumberManager::GetPlayerMiss() const {
	check(PlayerMissTexture);
	return PlayerMissTexture;
}
