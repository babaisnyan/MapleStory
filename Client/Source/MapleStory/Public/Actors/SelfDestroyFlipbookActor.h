#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "SelfDestroyFlipbookActor.generated.h"

UCLASS()
class MAPLESTORY_API ASelfDestroyFlipbookActor : public APaperFlipbookActor
{
	GENERATED_BODY()
	
public:
	ASelfDestroyFlipbookActor();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void DestroySelf();
};
