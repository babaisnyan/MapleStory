// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "MaplePlayerController.generated.h"

UCLASS()
class MAPLESTORY_API AMaplePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
protected:
	virtual void BeginPlay() override;
};
