// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StatusBarHud.generated.h"

class UImage;

UCLASS()
class MAPLESTORY_API UStatusBarHud : public UUserWidget {
	GENERATED_BODY()

public:
	void UpdateGauge(float HpPercent, float MpPercent) const;
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Hp;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> Mp;
};
