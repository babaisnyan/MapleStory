#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8 {
	Eqp UMETA(DisplayName = "장비"),
	Use UMETA(DisplayName = "소비"),
	Etc UMETA(DisplayName = "기타"),
};
