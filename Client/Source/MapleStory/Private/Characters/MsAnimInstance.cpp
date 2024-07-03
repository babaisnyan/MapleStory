#include "Characters/MsAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UMsAnimInstance::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!MovementComponent) return;
	
	Speed = MovementComponent->Velocity.Size();
	bIsJumping = MovementComponent->IsFalling();
}
