#pragma once

UENUM(BlueprintType)
enum class EKeyType : uint8 {
	None UMETA(DisplayName = "없음"),
	Item UMETA(DisplayName = "아이템"),
	Skill UMETA(DisplayName = "스킬"),
	
	Equipment UMETA(DisplayName = "장비"),
};
