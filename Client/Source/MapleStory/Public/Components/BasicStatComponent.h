#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAPLESTORY_API UBasicStatComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UBasicStatComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Hp = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MaxHp = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 PhysicalAttack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 PhysicalDefense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 PhysicalDefenseRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MagicalAttack = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MagicalDefense = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 MagicalDefenseRate = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Speed = 0.0f;
};
