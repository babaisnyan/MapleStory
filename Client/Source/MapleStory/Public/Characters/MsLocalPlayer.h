#pragma once

#include "CoreMinimal.h"
#include "Characters/MsPlayerBase.h"
#include "MsLocalPlayer.generated.h"

class UItemManager;
class UInventoryManager;
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
	void UpdateStatusBar() const;
	virtual void OnDamaged(int32 Damage) override;
	virtual void OnDead() override;
	virtual void OnRevive() override;

public:
	UFUNCTION(BlueprintCallable)
	void OnClickedRevive();
	
protected:
	void EnhancedMoveHorizontal(const FInputActionValue& Value);
	void EnhancedJump(const FInputActionValue& Value);
	void Enter(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

private:
	void CheckHitMob();
	
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
	TSubclassOf<UUserWidget> DeathNoticeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UStatusBarHud> StatusBarHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UQuickSlotWidget> QuickSlotWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UChatWidget> ChatWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> DeathNotice;

private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> EnterAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveHorizontalAction;
	
	UPROPERTY()
	TObjectPtr<USoundManager> SoundManager;

	UPROPERTY()
	TObjectPtr<UKeySettingManager> KeySettingManager;

	UPROPERTY()
	TObjectPtr<UInventoryManager> InventoryManager;

	UPROPERTY()
	TObjectPtr<UItemManager> ItemManager;

	float MovePacketSendTimer = 0.1f;
	FVector LastMovePacketLocation;
	bool bFirstSent = false;
	protocol::PlayerAnimation LastAnimationType = protocol::PLAYER_ANIMATION_UNSPECIFIED;
	bool bSentRevive = false;
	double LastAttackTime = 0.0;
};
