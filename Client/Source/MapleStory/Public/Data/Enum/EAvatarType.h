#pragma once

UENUM(BlueprintType)
enum class EAvatarType : uint8 {
	None,
	GoTalk UMETA(DisplayName = "GoTalk"),
	Serena UMETA(DisplayName = "Serena"),
	Luke UMETA(DisplayName = "Luke")
};
