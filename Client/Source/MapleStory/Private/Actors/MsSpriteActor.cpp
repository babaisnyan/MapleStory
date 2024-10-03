#include "Actors/MsSpriteActor.h"

#include "Components/MsSpriteComponent.h"

AMsSpriteActor::AMsSpriteActor() {
	PrimaryActorTick.bCanEverTick = true;

	SpriteComponent = CreateDefaultSubobject<UMsSpriteComponent>(TEXT("SpriteComponent"));
	SpriteComponent->OnFinishedPlaying.AddDynamic(this, &AMsSpriteActor::OnFinished);

	RootComponent = SpriteComponent;
}

void AMsSpriteActor::Setup(const UDataTable* SpriteTable, const int32 StartDelay) {
	if (!SpriteComponent) {
		return;
	}

	Delay = StartDelay * 0.9f / 1000.0f;
	SpriteComponent->Setup(SpriteTable, StartDelay == 0, false);
}

void AMsSpriteActor::SetPriority(const int32 Priority) const {
	SpriteComponent->SetPriority(Priority);
}

void AMsSpriteActor::BeginPlay() {
	Super::BeginPlay();

	if (Delay != 0) {
		SpriteComponent->SetVisibility(false);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMsSpriteActor::OnDelayFinished, Delay, false);
	}
}

void AMsSpriteActor::OnDelayFinished() {
	if (SpriteComponent) {
		SpriteComponent->SetVisibility(true);
		SpriteComponent->Play();
		GetWorldTimerManager().ClearTimer(TimerHandle);
	}
}

void AMsSpriteActor::OnFinished(UMsSpriteComponent* Discard) {
	Destroy();
}
