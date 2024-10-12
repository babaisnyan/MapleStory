#include "UI/MobHpBar.h"

#include "Components/ProgressBar.h"

void UMobHpBar::UpdateHpBar() const {
	HpBar->SetPercent(static_cast<float>(CurrentHp) / MaxHp);
	// log CurrentHp, MaxHp, CurrentHp / MaxHp
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CurrentHp: %d, MaxHp: %d, Percent: %f"), CurrentHp, MaxHp, static_cast<float>(CurrentHp) / MaxHp));
}
