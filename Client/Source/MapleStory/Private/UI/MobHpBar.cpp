#include "UI/MobHpBar.h"

#include "Components/ProgressBar.h"

void UMobHpBar::UpdateHpBar() const {
	HpBar->SetPercent(static_cast<float>(CurrentHp) / MaxHp);
}
