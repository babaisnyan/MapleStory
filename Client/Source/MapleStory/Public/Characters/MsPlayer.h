// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Data/Enum/EAvatarType.h"
#include "MsPlayer.generated.h"

class USoundManager;
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

protected:
	void EnhancedMoveHorizontal(const FInputActionValue& Value);
	void EnhancedMoveVertical(const FInputActionValue& Value);
	void EnhancedJump(const FInputActionValue& Value);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class APlayerCamera* PlayerCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UPaperFlipbook* RunAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UPaperFlipbook* JumpAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	EPlayerAnimationType AnimationType;

	UPROPERTY(EditDefaultsOnly, Category = "Avatar")
	EAvatarType AvatarType;

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
