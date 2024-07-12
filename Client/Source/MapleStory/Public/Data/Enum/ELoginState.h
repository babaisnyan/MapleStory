#pragma once

UENUM(BlueprintType)
enum class ELoginState : uint8 {
	Login UMETA(DisplayName = "Login"),
	CharacterSelection UMETA(DisplayName = "CharacterSelection"),
	CharacterCreation UMETA(DisplayName = "CharacterCreation"),
	InGame UMETA(DisplayName = "InGame")
};
