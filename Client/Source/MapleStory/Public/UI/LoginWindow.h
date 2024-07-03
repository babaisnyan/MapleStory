// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "LoginWindow.generated.h"

/**
 * 
 */
UCLASS()
class MAPLESTORY_API ULoginWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TryLogin();
};
