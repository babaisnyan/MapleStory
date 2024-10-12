#include "Actors/SelfDestroyFlipbookActor.h"

#include "PaperFlipbookComponent.h"

ASelfDestroyFlipbookActor::ASelfDestroyFlipbookActor() {
	GetRenderComponent()->TranslucencySortPriority = 1000002;
}

void ASelfDestroyFlipbookActor::BeginPlay() {
	Super::BeginPlay();
	GetRenderComponent()->OnFinishedPlaying.AddDynamic(this, &ASelfDestroyFlipbookActor::DestroySelf);
	GetRenderComponent()->Play();
}

void ASelfDestroyFlipbookActor::DestroySelf() {
	Destroy();
}
