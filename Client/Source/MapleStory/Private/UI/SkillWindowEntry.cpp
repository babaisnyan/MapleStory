#include "UI/SkillWindowEntry.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USkillWindowEntry::Setup(const int32 Id, const FString& Name, const int32 Level) {
	SkillId = Id;
	SkillLevel = Level;
	SkillName = Name;
	
	SkillNameText->SetText(FText::FromString(SkillName));
	SkillLevelText->SetText(FText::FromString(FString::FromInt(SkillLevel)));

	// TODO: 스킬 아이콘 가져오기

	FButtonStyle ButtonStyle;
	ButtonStyle.Normal.SetResourceObject(SkillIcon);
	ButtonStyle.Hovered.SetResourceObject(SkillIconHover);
	SkillButton->SetStyle(ButtonStyle);
}
