#pragma once

UENUM(BlueprintType)
enum class ESubItemType : uint8 {
	Accessory UMETA(DisplayName = "악세서리"),
	Cap UMETA(DisplayName = "모자"),
	Cape UMETA(DisplayName = "망토"),
	Coat UMETA(DisplayName = "상의"),
	Glove UMETA(DisplayName = "장갑"),
	Longcoat UMETA(DisplayName = "한벌옷"),
	Pants UMETA(DisplayName = "하의"),
	Ring UMETA(DisplayName = "반지"),
	Shield UMETA(DisplayName = "방패"),
	Shoes UMETA(DisplayName = "신발"),
	Weapon UMETA(DisplayName = "무기"),
};
