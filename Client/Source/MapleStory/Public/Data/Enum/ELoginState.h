#pragma once

UENUM(BlueprintType)
enum class ELoginState : uint8 {
	Login UMETA(DisplayName = "Login"),
	CharacterSelection UMETA(DisplayName = "CharacterSelection"),
	InGame UMETA(DisplayName = "InGame")
};
