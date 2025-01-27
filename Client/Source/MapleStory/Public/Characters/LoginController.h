
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoginController.generated.h"


UCLASS()
class MAPLESTORY_API ALoginController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> LoginWindow;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UUserWidget> Hud;
};
