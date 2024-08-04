#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "Data/Enum/EAvatarType.h"
#include "AvatarData.generated.h"

USTRUCT(BlueprintType, meta=(DeprecatedStructure, DeprecationMessage="옷입히기 구현시 삭제할 예정"))
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
};
