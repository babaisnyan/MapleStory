#pragma once

#include "CoreMinimal.h"
#include "Table/ItemTemplate.h"
#include "Item.generated.h"

UCLASS()
class MAPLESTORY_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	FItemTemplate Template;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Quantity;
};
