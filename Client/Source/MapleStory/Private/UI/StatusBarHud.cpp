// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatusBarHud.h"

#include "Components/Image.h"

void UStatusBarHud::UpdateGauge(const float HpPercent, const float MpPercent) const {
	Hp->SetRenderScale(FVector2D(HpPercent, 1.0f));
	Mp->SetRenderScale(FVector2D(MpPercent, 1.0f));
}
