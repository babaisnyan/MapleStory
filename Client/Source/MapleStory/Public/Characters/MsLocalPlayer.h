#pragma once

#include "CoreMinimal.h"
#include "Characters/MsPlayerBase.h"
#include "MsLocalPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USoundManager;
class UStatusBarHud;
class APlayerCamera;
struct FInputActionValue;

UCLASS()
class MAPLESTORY_API AMsLocalPlayer : public AMsPlayerBase {
	GENERATED_BODY()

public:
	AMsLocalPlayer();

public:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	void EnhancedMoveHorizontal(const FInputActionValue& Value);
	void EnhancedMoveVertical(const FInputActionValue& Value);
	void EnhancedJump(const FInputActionValue& Value);

private:
	void UpdateStatusBar() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<APlayerCamera> PlayerCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UStatusBarHud> StatusBarHudClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UStatusBarHud> StatusBarHud;
	
private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveHorizontalAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveVerticalAction;

	UPROPERTY()
	TObjectPtr<USoundManager> SoundManager;

	float MovePacketSendTimer = 0.05f;
};
