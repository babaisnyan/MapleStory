// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"
#include "MsPlayer.generated.h"

class UWidgetComponent;
class UStatusBarHud;
class UPlayerStatComponent;
class APlayerCamera;
class USoundManager;
class UPaperFlipbook;
enum class EPlayerAnimationType : uint8;
struct FInputActionValue;

/**
 * MapleStory Player
 */
UCLASS()
class MAPLESTORY_API AMsPlayer : public APaperZDCharacter {
	GENERATED_BODY()

public:
	AMsPlayer();

private:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void UpdateAnimation() const;
	void UpdateStatusBar() const;

protected:
	void EnhancedMoveHorizontal(const FInputActionValue& Value);
	void EnhancedMoveVertical(const FInputActionValue& Value);
	void EnhancedJump(const FInputActionValue& Value);

public:
	void Setup(const protocol::PlayerInfo& Info);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<APlayerCamera> PlayerCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> RunAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> JumpAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	EPlayerAnimationType AnimationType;

	UPROPERTY(EditDefaultsOnly, Category = "Avatar")
	EAvatarType AvatarType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UPlayerStatComponent> PlayerStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UStatusBarHud> StatusBarHudClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    TSoftObjectPtr<UStatusBarHud> StatusBarHud;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> NameTagWidget;
	
private:
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	UInputAction* MoveHorizontalAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	UInputAction* MoveVerticalAction;

	TObjectPtr<USoundManager> SoundManager;
};
