#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

class UPaperSprite;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeOutFinished);

class UDamageNumberManager;
class UHorizontalBox;

UCLASS()
class MAPLESTORY_API UDamageNumberWidget : public UUserWidget {
	GENERATED_BODY()

public:
	void SetDamageNumberMob(int32 Damage);
	void SetDamageNumberPlayer(int32 Damage, bool bCritical);
	void StartFadeOut();

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	void AddNumberImage(const UPaperSprite* Sprite);
	void AddMissImage(UTexture2D* Texture);

public:
	FOnFadeOutFinished OnFadeOutFinished;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> NumberContainer;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_FadeOut;
};
