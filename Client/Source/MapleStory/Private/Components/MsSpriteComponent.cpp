#include "Components/MsSpriteComponent.h"

#include "Data/Table/AnimationData.h"

UMsSpriteComponent::UMsSpriteComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	BodyInstance.bLockXRotation = true;
	BodyInstance.bLockYRotation = true;
	BodyInstance.bLockYTranslation = true;
}

void UMsSpriteComponent::Setup(const UDataTable* SpriteTable, const bool bAutoStart, const bool bLoop) {
	if (!SpriteTable) {
		return;
	}

	SetCollisionProfileName(TEXT("NoCollision"));
	CanCharacterStepUpOn = ECB_No;
	SetGenerateOverlapEvents(false);

	Sprites.Empty();
	Delays.Empty();
	Offsets.Empty();
	ZOrders.Empty();
	HasAlpha.Empty();
	AlphaStarts.Empty();
	AlphaEnds.Empty();

	bLooping = bLoop;
	bool bZigZag = false;

	for (const auto& Row : SpriteTable->GetRowNames()) {
		const FAnimationData* Data = SpriteTable->FindRow<FAnimationData>(Row, "");

		if (Data) {
			Sprites.Add(Data->Sprite.LoadSynchronous());
			Delays.Add(static_cast<float>(Data->Delay) / 1000.0f);
			Offsets.Add(FVector2D(Data->OffsetX, Data->OffsetY));
			ZOrders.Add(Data->ZOrder);
			HasAlpha.Add(Data->HasAlpha);
			AlphaStarts.Add(Data->AlphaStart);
			AlphaEnds.Add(Data->AlphaEnd);
			bZigZag = Data->bZigZag;
		}
	}

	check(!Sprites.IsEmpty());
	check(Sprites.Num() + Delays.Num() + Offsets.Num() + ZOrders.Num() + HasAlpha.Num() + AlphaStarts.Num() + AlphaEnds.Num() == Sprites.Num() * 7);

	if (bZigZag) {
		for (int32 i = Sprites.Num() - 2; i >= 0; --i) {
			Sprites.Add(Sprites[i]);
			Delays.Add(Delays[i]);
			Offsets.Add(Offsets[i]);
			ZOrders.Add(ZOrders[i]);
		}
	}

	bStarted = bAutoStart;

	SetSprite(Sprites[0]);
	const FVector2D Offset = GetSpriteSize();
	SetRelativeLocation(FVector(0, BaseOffset.Y + ZOrders[CurrentIndex], -Offset.Y), false, nullptr, ETeleportType::ResetPhysics);
}

void UMsSpriteComponent::Reset() {
	bStarted = false;
	bEnded = false;
	CurrentIndex = 0;
	TimeElapsed = 0.0f;
	SetSprite(Sprites[0]);
}

void UMsSpriteComponent::Play() {
	SetSprite(Sprites[0]);
	const FVector2D Offset = GetSpriteSize();
	SetRelativeLocation(FVector(0, BaseOffset.Y + ZOrders[CurrentIndex], -Offset.Y), false, nullptr, ETeleportType::ResetPhysics);
	bStarted = true;

	OnCollisionUpdate.Broadcast(Offset);
}

void UMsSpriteComponent::BeginPlay() {
	Super::BeginPlay();

	check(Sprites.Num() + Delays.Num() + Offsets.Num() + ZOrders.Num() == Sprites.Num() * 4);

	BaseOffset = GetRelativeLocation();

	if (Sprites.Num() > 0) {
		SetSprite(Sprites[0]);
	}
}

void UMsSpriteComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	if (!bStarted) {
		return;
	}

	if (!bEnded && Sprites.Num() > 1) {
		TimeElapsed += DeltaTime;

		if (TimeElapsed >= Delays[CurrentIndex]) {
			if (!bLooping && CurrentIndex == Sprites.Num() - 1) {
				bEnded = true;
			} else {
				TimeElapsed = 0.0f;
				CurrentIndex = (CurrentIndex + 1) % Sprites.Num();
				SetSprite(Sprites[CurrentIndex]);
				
				// const FVector2D Size = GetCollisionSize();
				// OnCollisionUpdate.Broadcast(Size);
			}

			if (HasAlpha[CurrentIndex]) {
				const float Alpha = FMath::Lerp(AlphaStarts[CurrentIndex], AlphaEnds[CurrentIndex], TimeElapsed / Delays[CurrentIndex]);
				SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, Alpha / 255.0f));

				if (Alpha < 10.0f) {
					bEnded = true;
				}
			} else {
				SetSpriteColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnded) {
		OnFinishedPlaying.Broadcast(this);
	}
}

FVector2D UMsSpriteComponent::GetSpriteSize() {
	if (const UPaperSprite* Sprite = GetSprite()) {
		return FVector2D(Sprite->GetSourceSize().X / 2, Sprite->GetSourceSize().Y / 2);
	}

	return FVector2D::ZeroVector;
}

FVector2D UMsSpriteComponent::GetAdjustOffset() {
	const FVector2D Size = GetSpriteSize();
	const FVector2D Offset = Offsets[CurrentIndex];
	const int32 X = Offset.X > Size.X ? Offset.X - Size.X : Size.X - Offset.X;
	const int32 Y = Offset.Y > Size.Y ? Offset.Y - Size.Y : Size.Y - Offset.Y;

	return FVector2D(X, -Y);
}
