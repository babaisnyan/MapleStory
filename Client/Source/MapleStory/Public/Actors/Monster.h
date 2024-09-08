#pragma once

#include "CoreMinimal.h"
#include "game_protocol.pb.h"
#include "Data/Enum/EMobActionType.h"
#include "GameFramework/Actor.h"
#include "Monster.generated.h"

class UNavMovementComponent;
class UMobStatComponent;
class UBoxComponent;
struct FAnimationData;
struct FMobTemplate;
class UMsSpriteComponent;

UCLASS(meta = (ChildCanTick))
class MAPLESTORY_API AMonster : public APawn {
	GENERATED_BODY()

public:
	AMonster();

	bool Init(int32 Id, int64 ObjId, float Y = 0.0f, EMobActionType ActionType = EMobActionType::Stand, bool Flip = false);

	EMobActionType GetCurrentAction() const {
		return CurrentAction;
	}

	void SetCurrentAction(EMobActionType ActionType, bool bForce = false);
	void Move(const protocol::GameServerMobMove& Packet);

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void Setup(const FMobTemplate* MobTemplate);
	void AddAnimation(EMobActionType ActionType, const FString& ActionName, bool bRegisterEvent = true);

	UFUNCTION()
	void OnFinishedPlaying(UMsSpriteComponent* SpriteComponent);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCollisionUpdate(const FVector2D Size);

public:
	UPROPERTY(VisibleAnywhere, Category = "Mob")
	TObjectPtr<UBoxComponent> BoxComponent;

	TObjectPtr<UPawnMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mob")
	TMap<EMobActionType, TObjectPtr<UMsSpriteComponent>> SpriteComponents;

	UPROPERTY(VisibleAnywhere, Category = "Mob")
	TObjectPtr<UMobStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mob")
	int64 ObjectId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 MobId = 100100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	FName MobName = TEXT("None");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool BodyAttack = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	bool FirstAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 Exp = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 AttackWidth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mob")
	int32 AttackHeight;

	UPROPERTY(EditAnywhere, Category = "Mob")
	EMobActionType CurrentAction = EMobActionType::None;

	UPROPERTY(EditAnywhere, Category = "Mob")
	bool bCanDamaged = true;

	UPROPERTY(EditAnywhere, Category = "Mob")
	bool bFlip = false;

private:
	int32 DestX;
	float DestY;
	float BaseX;
	float BaseY;
	int32 ZIndex;
};
