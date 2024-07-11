#pragma once

UENUM(BlueprintType)
enum class EJobType : uint8 {
	Admin UMETA(DisplayName = "운영자"),
	Error = 100 UMETA(DisplayName = "오류"),
};
