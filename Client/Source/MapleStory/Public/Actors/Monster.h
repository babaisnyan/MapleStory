#pragma once

#include "CoreMinimal.h"
#include "Data/Enum/EMobActionType.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

class UBoxComponent;
struct FAnimationData;
struct FMobTemplate;
class UMsSpriteComponent;

UCLASS(meta = (ChildCanTick))
class MAPLESTORY_API AMonster : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonster();

	void Init(int32 Id);

	EMobActionType GetCurrentAction() const { return CurrentAction; }
	void SetCurrentAction(EMobActionType ActionType, bool bForce = false);
	
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void Setup(const FMobTemplate* MobTemplate);
	void AddAnimation(EMobActionType ActionType, const FString& ActionName);
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Mob")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UPROPERTY(VisibleAnywhere, Category = "Mob")
	TMap<EMobActionType, TObjectPtr<UMsSpriteComponent>> SpriteComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MobId = 100100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	FName MobName = TEXT("None");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Hp = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool BodyAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool FirstAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Speed = -50;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 PaDamage = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MaDamage = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 PdDamage = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MdDamage = 01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 PdRate = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MdRate = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Exp = 0;
	
	UPROPERTY(EditAnywhere, Category = "Mob")
	EMobActionType CurrentAction = EMobActionType::Regen;
};
