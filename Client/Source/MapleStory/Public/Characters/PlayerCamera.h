#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Components/TimelineComponent.h"
#include "PlayerCamera.generated.h"


class AMsLocalPlayer;

UCLASS()
class MAPLESTORY_API APlayerCamera : public ACameraActor {
	GENERATED_BODY()

public:
	APlayerCamera();

public:
	UFUNCTION()
	void OnTimelineUpdate(float Value);

protected:
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AMsLocalPlayer> Player;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCurveFloat> Curve;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	FTimeline Timeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bStopCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraMoveDelaySeconds;
};
