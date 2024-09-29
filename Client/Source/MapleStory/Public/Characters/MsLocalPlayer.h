#pragma once

#include "CoreMinimal.h"
#include "Characters/MsPlayerBase.h"
#include "MsLocalPlayer.generated.h"

class UChatWidget;
class UKeySettingManager;
class UQuickSlotWidget;
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

	virtual void Setup(const protocol::PlayerInfo& Info) override;
	virtual void OnDamaged(int32 Damage) override;

protected:
	void EnhancedMoveHorizontal(const FInputActionValue& Value);
	void EnhancedMoveVertical(const FInputActionValue& Value);
	void EnhancedJump(const FInputActionValue& Value);
	void Enter(const FInputActionValue& Value);

private:
	void UpdateStatusBar() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<APlayerCamera> PlayerCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UStatusBarHud> StatusBarHudClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UQuickSlotWidget> QuickSlotWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UChatWidget> ChatWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UStatusBarHud> StatusBarHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UQuickSlotWidget> QuickSlotWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UChatWidget> ChatWidget;

private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> EnterAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveHorizontalAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveVerticalAction;

	UPROPERTY()
	TObjectPtr<USoundManager> SoundManager;

	UPROPERTY()
	TObjectPtr<UKeySettingManager> KeySettingManager;

	float MovePacketSendTimer = 0.1f;
	FVector LastMovePacketLocation;
	protocol::PlayerAnimation LastAnimationType = protocol::PLAYER_ANIMATION_UNSPECIFIED;
};
