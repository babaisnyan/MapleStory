#pragma once

UENUM(BlueprintType)
enum class EKeyType : uint8 {
	None UMETA(DisplayName = "없음"),
	Item UMETA(DisplayName = "아이템"),
	Skill UMETA(DisplayName = "스킬"),

	CharInfo UMETA(DisplayName = "캐릭터 정보"),
	Equipment UMETA(DisplayName = "장비"),
	Inventory UMETA(DisplayName = "인벤토리"),
	SkillWindow UMETA(DisplayName = "스킬 창"),
	Attack UMETA(DisplayName = "공격"),
	Jump UMETA(DisplayName = "점프"),
	KeyConfig UMETA(DisplayName = "키 설정"),
	Menu UMETA(DisplayName = "메뉴"),
	NpcTalk UMETA(DisplayName = "NPC 대화"),
	PickUp UMETA(DisplayName = "줍기")
};
