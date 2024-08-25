#pragma once

UENUM(BlueprintType)
enum class EMobActionType : uint8 {
	Stand UMETA(DisplayName = "Stand"),
	Move UMETA(DisplayName = "Move"),
	Hit UMETA(DisplayName = "Hit"),
	Die UMETA(DisplayName = "Die"),
	Regen UMETA(DisplayName = "Regen")
};
