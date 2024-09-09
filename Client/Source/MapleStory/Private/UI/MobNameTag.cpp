#include "UI/MobNameTag.h"

#include "Components/TextBlock.h"

void UMobNameTag::SetInfo(const int32 Level, const FName& Name) const {
	const FString Info = FString::Printf(TEXT("Lv.%d %s"), Level, *Name.ToString());
	MobName->SetText(FText::FromString(Info));
}
