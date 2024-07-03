#pragma once

UENUM(BlueprintType)
enum class EPlayerAnimationType : uint8 {
	Idle UMETA(DisplayName = "Idle"),
	Run UMETA(DisplayName = "Run"),
	Jump UMETA(DisplayName = "Jump"),
	Fall UMETA(DisplayName = "Fall")
};
