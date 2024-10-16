

#include "UI/StatusBarHud.h"

#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void UStatusBarHud::UpdateGauge(const int32 Hp, const int32 MaxHp, const int32 Mp, const int32 MaxMp, const int32 Exp, const int32 MaxExp) const {
	const float HpRatio = static_cast<float>(Hp) / MaxHp;
	const float MpRatio = static_cast<float>(Mp) / MaxMp;
	const float ExpRatio = static_cast<float>(Exp) / MaxExp;

	HpImage->SetRenderScale(FVector2D(HpRatio, 1.0f));
	MpImage->SetRenderScale(FVector2D(MpRatio, 1.0f));
	ExpImage->SetRenderScale(FVector2D(ExpRatio, 1.0f));
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Hp, MaxHp)));
	MpText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Mp, MaxMp)));

	FString ExpTextFormat = TEXT("");
	FString ExpString = FString::Printf(TEXT("%d"), Exp);
	FString ExpPercentageString = FString::Printf(TEXT("%.3f"), ExpRatio * 100.0f);

	for (int32 i = ExpString.Len() - 3; i > 0; i -= 3) {
		ExpString.InsertAt(i, TEXT(","));
	}

	for (int32 i = 0; i < ExpString.Len(); i++) {
		if (ExpString[i] == ',') {
			ExpTextFormat += FString::Printf(TEXT("<img id=\"ExpTextComma\" width=\"desired\"/>"));
		} else {
			ExpTextFormat += FString::Printf(TEXT("<img id=\"ExpText%d\" width=\"desired\"/>"), ExpString[i] - '0');
		}
	}

	ExpTextFormat += TEXT("<img id=\"ExpText[\" width=\"desired\"/>");

	for (int32 i = 0; i < ExpPercentageString.Len(); i++) {
		if (ExpPercentageString[i] == '.') {
			ExpTextFormat += TEXT("<img id=\"ExpTextDot\" width=\"desired\"/>");
		} else {
			ExpTextFormat += FString::Printf(TEXT("<img id=\"ExpText%d\" width=\"desired\"/>"), ExpPercentageString[i] - '0');
		}
	}

	ExpTextFormat += TEXT("<img id=\"ExpText%\" width=\"desired\"/>");
	ExpTextFormat += TEXT("<img id=\"ExpText]\" width=\"desired\"/>");

	ExpText->SetText(FText::FromString(ExpTextFormat));
}

void UStatusBarHud::UpdateLevel(const int32 Level) const {
	FString LevelTextFormat = FString::Printf(TEXT("%d"), Level);
	FString Text = TEXT("");

	for (int32 i = 0; i < LevelTextFormat.Len(); i++) {
		Text += FString::Printf(TEXT("<img id=\"LevelText%d\" width=\"desired\"/>"), LevelTextFormat[i] - '0');
	}

	LevelText->SetText(FText::FromString(Text));
}

void UStatusBarHud::SetName(const FString& Name) const {
	NameText->SetText(FText::FromString(Name));
}
