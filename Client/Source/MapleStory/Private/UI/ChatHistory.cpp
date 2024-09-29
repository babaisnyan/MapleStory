#include "UI/ChatHistory.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"

void UChatHistory::SetText(const FString& InText) const {
	Text->SetText(FText::FromString(InText));
}

void UChatHistory::SetText(const FString& InText, const FColor& BackColor, const FColor& TextColor) const {
	Text->SetText(FText::FromString(InText));
	Text->SetColorAndOpacity(TextColor);
	Back->SetBrushColor(BackColor);
}
