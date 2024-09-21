#include "Actors/DamageTextActor.h"

#include "PoolManagerSubsystem.h"
#include "Components/WidgetComponent.h"
#include "UI/DamageNumberWidget.h"

ADamageTextActor::ADamageTextActor() {
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UDamageNumberWidget> WidgetClassFinder(TEXT("/Game/UI/Common/WBP_Damage.WBP_Damage_C"));

	if (WidgetClassFinder.Class != nullptr) {
		DamageWidgetClass = WidgetClassFinder.Class;

		DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
		DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		DamageWidgetComponent->SetDrawSize(FVector2D(200.0f, 100.0f));
		DamageWidgetComponent->SetDrawAtDesiredSize(true);
		DamageWidgetComponent->SetWidgetClass(DamageWidgetClass);

		RootComponent = DamageWidgetComponent;
	}
}

void ADamageTextActor::SetDamage(const int32 Damage, const bool bMob, const bool bCritical) {
	DamageValue = Damage;
	bIsMob = bMob;
	bIsCritical = bCritical;
}

void ADamageTextActor::BeginPlay() {
	Super::BeginPlay();

	if (!DamageWidgetComponent) {
		return;
	}

	if (UDamageNumberWidget* DamageWidget = Cast<UDamageNumberWidget>(DamageWidgetComponent->GetUserWidgetObject())) {
		if (bIsMob) {
			DamageWidget->SetDamageNumberMob(DamageValue);
		} else {
			DamageWidget->SetDamageNumberPlayer(DamageValue, bIsCritical);
		}

		DamageWidget->StartFadeOut();
		DamageWidget->OnFadeOutFinished.AddDynamic(this, &ADamageTextActor::OnFadeOutFinished);
	}
}

void ADamageTextActor::OnFadeOutFinished() {
	Destroy();
}
