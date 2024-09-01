#pragma once

#include "CoreMinimal.h"
#include "NpcTemplate.generated.h"

USTRUCT(BlueprintType)
struct MAPLESTORY_API FNpcAction {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	int32 Length;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	TArray<FString> Scripts;
};

USTRUCT(BlueprintType)
struct MAPLESTORY_API FNpcTemplate : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	int32 Id = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	FString NpcName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	TMap<FString, FString> Texts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	TMap<FString, FNpcAction> Actions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Npc")
	bool IsShop;
};
