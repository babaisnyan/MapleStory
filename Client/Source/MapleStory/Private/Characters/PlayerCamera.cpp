#include "Characters/PlayerCamera.h"

#include "EngineUtils.h"
#include "PaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/MsLocalPlayer.h"

#include "GameFramework/SpringArmComponent.h"

APlayerCamera::APlayerCamera() {
	PrimaryActorTick.bCanEverTick = true;

	UCameraComponent* Camera = GetCameraComponent();
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 130.0f));
	SpringArm->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bDoCollisionTest = false;

	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	Camera->SetOrthoWidth(1304);
	Camera->SetAutoCalculateOrthoPlanes(false);
	Camera->SetOrthoNearClipPlane(-100000.0f);

	const ConstructorHelpers::FObjectFinder<UCurveFloat> CurveFinder(TEXT("/Script/Engine.CurveFloat'/Game/Misc/C_CameraPosition.C_CameraPosition'"));

	if (CurveFinder.Succeeded()) {
		Curve = CurveFinder.Object;
		Timeline = {};
		Timeline.SetTimelineLength(1.0f);
		Timeline.SetTimelineLengthMode(TL_LastKeyFrame);
		Timeline.SetPlayRate(1.5f);

		FOnTimelineFloat TimelineUpdate;
		TimelineUpdate.BindUFunction(this, FName("OnTimelineUpdate"));
		Timeline.AddInterpFloat(Curve, TimelineUpdate);

		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));
		Timeline.SetTimelineFinishedFunc(TimelineFinished);
	}
}

void APlayerCamera::OnTimelineUpdate(const float Value) {
	const FVector PlayerLocation = Player.IsValid() ? Player->GetActorLocation() : FVector::ZeroVector;
	UCameraComponent* Camera = GetCameraComponent();
	FVector CameraLocation = GetActorLocation();

	// const FVector NewLocation = FMath::Lerp(CameraLocation, PlayerLocation, Value);
	//
	// SetActorLocation(NewLocation);

	if (Camera && TileMapComponent) {
		int32 Width = 0;
		int32 Height = 0;
		int32 LayerCount = 0;
		TileMapComponent->GetMapSize(Width, Height, LayerCount);
		const FVector2D TileMapSize = FVector2D(Width * 30, Height * 30);
		const FVector TileMapOrigin = TileMapComponent->GetComponentLocation();

		const float OrthoWidth = Camera->OrthoWidth;
		const float OrthoHeight = Camera->OrthoWidth / Camera->AspectRatio;

		const float MinX = TileMapOrigin.X + OrthoWidth * 0.5f;
		const float MaxX = TileMapOrigin.X + TileMapSize.X - OrthoWidth * 0.5f;
		const float MinY = TileMapOrigin.Z + OrthoHeight * 0.5f;
		const float MaxY = TileMapOrigin.Z + TileMapSize.Y - OrthoHeight * 0.5f;
		const float X = CameraLocation.X;
		const float Y = CameraLocation.Z;

		CameraLocation.X = FMath::Clamp(X, MinX + 5.0f, MaxX - 15.0f);
		FVector NewLocation = FMath::Lerp(CameraLocation, PlayerLocation, Value);
		NewLocation.Z = FMath::Lerp(Y, PlayerLocation.Z, Value);
		SetActorLocation(NewLocation);
	}
}

void APlayerCamera::OnTimelineFinished() {
	CameraMoveDelaySeconds = 1.0f;
}

void APlayerCamera::BeginPlay() {
	Super::BeginPlay();

	for (TActorIterator<APaperTileMapActor> It(GetWorld()); It; ++It) {
		TileMapComponent = It->GetRenderComponent();
		if (TileMapComponent) {
			break;
		}
	}
}

void APlayerCamera::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	if (!bStopCamera) {
		if (CameraMoveDelaySeconds > 0.0f) {
			CameraMoveDelaySeconds -= DeltaSeconds;
			return;
		}

		Timeline.PlayFromStart();
		Timeline.TickTimeline(DeltaSeconds);
	}
}
