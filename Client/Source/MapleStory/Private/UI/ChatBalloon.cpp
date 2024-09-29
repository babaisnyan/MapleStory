#include "UI/ChatBalloon.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UChatBalloon::SetText(const FString& InText) const {
	Text->SetText(FText::FromString(InText));
}

void UChatBalloon::NativePreConstruct() {
	Super::NativePreConstruct();

	TopLeftImage->SetBrushFromTexture(TopLeft);
	TopImage->SetBrushFromTexture(Top);
	TopRightImage->SetBrushFromTexture(TopRight);
	LeftImage->SetBrushFromTexture(Left);
	CenterImage->SetBrushFromTexture(Center);
	RightImage->SetBrushFromTexture(Right);
	BottomLeftImage->SetBrushFromTexture(BottomLeft);
	BottomImage->SetBrushFromTexture(Bottom);
	BottomRightImage->SetBrushFromTexture(BottomRight);
	ArrowImage->SetBrushFromTexture(Arrow);
	Text->SetColorAndOpacity(Color);
}
