#include "Characters/MaplePlayerController.h"

#include "Managers/MsCheatManager.h"

AMaplePlayerController::AMaplePlayerController() {
	CheatClass = UMsCheatManager::StaticClass();
}

void AMaplePlayerController::BeginPlay() {
	Super::BeginPlay();
	bShowMouseCursor = true;
}
