#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "MaplePlayerController.generated.h"

UCLASS()
class MAPLESTORY_API AMaplePlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AMaplePlayerController();

protected:
	virtual void BeginPlay() override;
};
