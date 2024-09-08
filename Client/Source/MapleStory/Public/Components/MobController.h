#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MobController.generated.h"

UCLASS()
class MAPLESTORY_API AMobController : public AAIController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* InPawn) override;
};
