// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "MsAnimInstance.generated.h"

class UPawnMovementComponent;
class AMsPlayer;
/**
 * 
 */
UCLASS()
class MAPLESTORY_API UMsAnimInstance : public UPaperZDAnimInstance {
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AMsPlayer> Player;

	UPROPERTY(BlueprintReadWrite, Category = Movement)
	TObjectPtr<UPawnMovementComponent> MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bIsJumping;
};
