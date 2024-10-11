#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Data/Enum/EAvatarType.h"
#include "AvatarData.generated.h"

USTRUCT(BlueprintType)
struct MAPLESTORY_API FAvatarData : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	EAvatarType AvatarType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	TObjectPtr<UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	TObjectPtr<UPaperFlipbook> WalkAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	TObjectPtr<UPaperFlipbook> JumpAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Avatar")
	TObjectPtr<UPaperFlipbook> AttackAnimation;
};
