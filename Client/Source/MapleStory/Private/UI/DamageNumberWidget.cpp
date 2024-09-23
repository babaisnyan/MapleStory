#include "UI/DamageNumberWidget.h"

#include "PaperSprite.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Managers/DamageNumberManager.h"

void UDamageNumberWidget::SetDamageNumberMob(const int32 Damage) {
	UDamageNumberManager* NumberManager = GetGameInstance()->GetSubsystem<UDamageNumberManager>();

	if (!NumberManager) {
		return;
	}

	NumberContainer->ClearChildren();

	if (Damage <= 0) {
		AddNumberImage(NumberManager->GetMobMiss());
		return;
	}

	FString DamageString = FString::FromInt(Damage);

	for (const TCHAR Digit : DamageString) {
		const int32 NumericDigit = Digit - '0';
		const UPaperSprite* Sprite = NumberManager->GetMobNumber(NumericDigit);
		AddNumberImage(Sprite);
	}
}

void UDamageNumberWidget::SetDamageNumberPlayer(const int32 Damage, bool bCritical) {
	UDamageNumberManager* NumberManager = GetGameInstance()->GetSubsystem<UDamageNumberManager>();

	if (!NumberManager) {
		return;
	}

	NumberContainer->ClearChildren();

	if (Damage <= 0) {
		AddNumberImage(NumberManager->GetMobMiss());
		return;
	}

	FString DamageString = FString::FromInt(Damage);

	for (const TCHAR Digit : DamageString) {
		const int32 NumericDigit = Digit - '0';
		AddNumberImage(bCritical ? NumberManager->GetCriticalPlayerNumber(NumericDigit) : NumberManager->GetPlayerNumber(NumericDigit));
	}
}

void UDamageNumberWidget::StartFadeOut() {
	if (Anim_FadeOut) {
		PlayAnimation(Anim_FadeOut);
	}
}

void UDamageNumberWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) {
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation == Anim_FadeOut) {
		OnFadeOutFinished.Broadcast();
	}
}

void UDamageNumberWidget::AddNumberImage(const UPaperSprite* Sprite, const bool bMiss) {
	check(Sprite);

	UImage* DigitImage = NewObject<UImage>(this);
	DigitImage->SetBrushFromTexture(Sprite->GetSourceTexture());
	DigitImage->SetRenderTransformPivot(Sprite->GetPivotPosition());
	NumberContainer->AddChild(DigitImage);

	if(bMiss) {
		
	}
}