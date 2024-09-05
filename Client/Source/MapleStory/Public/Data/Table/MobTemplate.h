#pragma once

#include "CoreMinimal.h"
#include "MobTemplate.generated.h"

USTRUCT(BlueprintType)
struct MAPLESTORY_API FMobTemplate : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MobId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	FName MobName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MaxHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool BodyAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool FirstAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 PaDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MaDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 PdDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MdDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 PdRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MdRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Exp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool HasStand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool HasMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool HasHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool HasDie;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool HasRegen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool HasAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 AttackWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 AttackHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	TSoftObjectPtr<USoundWave> CharHitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	TSoftObjectPtr<USoundWave> DieSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	TSoftObjectPtr<USoundWave> HitSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	TSoftObjectPtr<USoundWave> RegenSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	TSoftObjectPtr<USoundWave> AttackSound;
};
