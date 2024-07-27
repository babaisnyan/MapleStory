#pragma once

#include "CoreMinimal.h"
#include "PaperSprite.h"
#include "Data/Enum/EItemType.h"
#include "Data/Enum/ESubItemType.h"
#include "ItemTemplate.generated.h"

USTRUCT(BlueprintType)
struct MAPLESTORY_API FItemTemplate : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ItemId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UPaperSprite> Icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UPaperSprite> IconRaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FString Desc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	ESubItemType SubType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncAcc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncDex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncEva;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncJump;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncLuk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncMad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncMdd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncMhp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncPad;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncPdd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Price;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 SlotMax;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ReqLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ReqStr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ReqDex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ReqInt;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 ReqLuk;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncStr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 IncMmd;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Hp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Mp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 Time;
};
