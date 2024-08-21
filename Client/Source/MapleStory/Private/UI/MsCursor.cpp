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

void UMsCursor::Clear() {
	PrevKeyWidget = nullptr;
	KeyType = EKeyType::None;
	ItemId = 0;
	ItemCount = 0;
	SkillId = 0;
}

void UMsCursor::ApplyCursorFlipbook() const {
	if (CursorFlipbook && CursorImage) {
		CursorImage->SetFlipbook(CursorFlipbook);
	}
}
