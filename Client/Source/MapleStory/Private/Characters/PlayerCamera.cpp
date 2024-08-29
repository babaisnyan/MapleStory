
#include "Characters/PlayerCamera.h"

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
	}
}

void APlayerCamera::OnTimelineUpdate(const float Value) {
	const FVector PlayerLocation = Player.IsValid() ? Player->GetActorLocation() : FVector::ZeroVector;
	const FVector CameraLocation = GetActorLocation();
	const FVector NewLocation = FMath::Lerp(CameraLocation, PlayerLocation, Value);
	
	SetActorLocation(NewLocation);
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
