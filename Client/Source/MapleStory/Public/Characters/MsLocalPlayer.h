#pragma once

#include "CoreMinimal.h"
#include "Characters/MsPlayerBase.h"
#include "Data/Enum/EKeyCode.h"
#include "UI/EquipWindow.h"
#include "UI/InventoryWindow.h"
#include "UI/StatWindow.h"
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

private:
	enum class EAction : uint8 {
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		A,
		Attack,
		Delete,
		End,
		Enter,
		Equip,
		Home,
		Insert,
		Inventory,
		Jump,
		MoveHorizontal,
		PageDown,
		PageUp,
		Q,
		R,
		Shift,
		Stat,
		T,
		W
	};

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
	void Equip(const FInputActionValue& Value);
	void Inventory(const FInputActionValue& Value);
	void Stat(const FInputActionValue& Value);
	void Num1(const FInputActionValue& Value);
	void Num2(const FInputActionValue& Value);
	void Num3(const FInputActionValue& Value);
	void Num4(const FInputActionValue& Value);
	void Num5(const FInputActionValue& Value);
	void Num6(const FInputActionValue& Value);
	void A(const FInputActionValue& Value);
	void Delete(const FInputActionValue& Value);
	void End(const FInputActionValue& Value);
	void Home(const FInputActionValue& Value);
	void Insert(const FInputActionValue& Value);
	void PageDown(const FInputActionValue& Value);
	void PageUp(const FInputActionValue& Value);
	void Q(const FInputActionValue& Value);
	void R(const FInputActionValue& Value);
	void Shift(const FInputActionValue& Value);
	void T(const FInputActionValue& Value);
	void W(const FInputActionValue& Value);

private:
	void CheckHitMob();
	void OnQuickSlotKey(EKeyCode Action);

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
	TSubclassOf<UEquipWindow> EquipWindowClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UInventoryWindow> InventoryWindowClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UStatWindow> StatWindowClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UStatusBarHud> StatusBarHud;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UQuickSlotWidget> QuickSlotWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UChatWidget> ChatWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UUserWidget> DeathNotice;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	int32 Meso = 0;

private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputMappingContext> DefaultContext;
	
	TMap<EAction, TObjectPtr<UInputAction>> Actions;
	TMap<EKeyCode, double> LastKeyTime;

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
