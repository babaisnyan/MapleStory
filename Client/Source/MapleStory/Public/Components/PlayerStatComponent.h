// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicStatComponent.h"
#include "game_struct.pb.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MAPLESTORY_API UPlayerStatComponent : public UBasicStatComponent {
	GENERATED_BODY()

public:
	UPlayerStatComponent();

	void Setup(const protocol::PlayerInfo& Info);
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Str = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Dex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Int = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	int32 Luk = 0;
};
