// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"
#include "Data/Enum/EPlayerAnimationType.h"
#include "MsPlayerBase.generated.h"


class UWidgetComponent;
class UPlayerStatComponent;
class UPaperFlipbook;

UCLASS()
class MAPLESTORY_API AMsPlayerBase : public APaperZDCharacter {
	GENERATED_BODY()

public:
	AMsPlayerBase();

public:
	virtual void BeginPlay() override;

	/*로컬 플레이어용 */
	void Setup(const protocol::PlayerInfo& Info);

	/*다른 플레이어용 */
	void Setup(const protocol::OtherPlayerInfo& Info);

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	void InitAnimation();
	void UpdateAnimation() const;

public:
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 X;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 Y;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> NameTagWidget;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> AvatarTable;
};
