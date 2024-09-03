#pragma once

UENUM(BlueprintType)
enum class EMobActionType : uint8 {
	None UMETA(DisplayName = "None"),
	Stand UMETA(DisplayName = "Stand"),
	Move UMETA(DisplayName = "Move"),
	Hit UMETA(DisplayName = "Hit"),
	Attack UMETA(DisplayName = "Attack"),
	Die UMETA(DisplayName = "Die"),
	Regen UMETA(DisplayName = "Regen")
};
