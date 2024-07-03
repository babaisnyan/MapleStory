// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/GameInstance.h"
#include "Public/Data/Enum/ELoginState.h"

#include "MapleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAPLESTORY_API UMapleGameInstance : public UGameInstance {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	
	void ChangeLoginState(ELoginState NewState);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELoginState LoginState = ELoginState::Login;
};
