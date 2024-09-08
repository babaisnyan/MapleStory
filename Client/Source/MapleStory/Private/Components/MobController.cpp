#include "Components/MobController.h"

void AMobController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);

	// print pawn name
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Possessing %s"), *InPawn->GetName()));
}
