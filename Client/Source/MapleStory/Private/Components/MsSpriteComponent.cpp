#include "Components/MsSpriteComponent.h"

#include "Data/Table/AnimationData.h"

void UMsSpriteComponent::Setup(const UDataTable* SpriteTable, const bool bZigZag) {
	if (!SpriteTable) {
		return;
	}

	Sprites.Empty();
	Delays.Empty();
	Offsets.Empty();
	ZOrders.Empty();

	for (const auto& Row : SpriteTable->GetRowNames()) {
		const FAnimationData* Data = SpriteTable->FindRow<FAnimationData>(Row, "");

		if (Data) {
			Sprites.Add(Data->Sprite.LoadSynchronous());
			Delays.Add(static_cast<float>(Data->Delay) / 1000.0f);
			Offsets.Add(FVector2D(Data->OffsetX, Data->OffsetY));
			ZOrders.Add(Data->ZOrder);
		}
	}

	check(Sprites.Num() + Delays.Num() + Offsets.Num() + ZOrders.Num() == Sprites.Num() * 4);

	if (bZigZag) {
		for (int32 i = Sprites.Num() - 2; i >= 0; --i) {
			Sprites.Add(Sprites[i]);
			Delays.Add(Delays[i]);
			Offsets.Add(Offsets[i]);
			ZOrders.Add(ZOrders[i]);
		}
	}

	SetSprite(Sprites[0]);
	SetRelativeLocation(FVector(BaseOffset.X + Offsets[0].X, BaseOffset.Y + ZOrders[0], BaseOffset.Z + Offsets[0].Y), false, nullptr, ETeleportType::ResetPhysics);
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
	if (Sprites.Num() > 1) {
		TimeElapsed += DeltaTime;

		if (TimeElapsed >= Delays[CurrentIndex]) {
			TimeElapsed = 0.0f;
			CurrentIndex = (CurrentIndex + 1) % Sprites.Num();
			SetSprite(Sprites[CurrentIndex]);
			SetRelativeLocation(FVector(BaseOffset.X + Offsets[CurrentIndex].X, BaseOffset.Y + ZOrders[CurrentIndex], BaseOffset.Z + Offsets[CurrentIndex].Y), false, nullptr, ETeleportType::ResetPhysics);
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
