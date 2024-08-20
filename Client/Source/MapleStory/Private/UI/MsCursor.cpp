#include "UI/MsCursor.h"

#include "PaperFlipbookUserWidget.h"
#include "Components/Image.h"

void UMsCursor::ClearImage() {
	AttachTexture = nullptr;
	AttachImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
}

void UMsCursor::Attach(UTexture2D* InSprite) {
	if (InSprite && AttachImage) {
		AttachTexture = InSprite;
		AttachImage->SetBrushFromTexture(AttachTexture, false);
		AttachImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

void UMsCursor::ApplyCursorFlipbook() const {
	if (CursorFlipbook && CursorImage) {
		CursorImage->SetFlipbook(CursorFlipbook);
	}
}
