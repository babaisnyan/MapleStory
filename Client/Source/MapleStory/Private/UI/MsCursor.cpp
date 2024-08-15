#include "UI/MsCursor.h"

#include "PaperFlipbookUserWidget.h"


void UMsCursor::ApplyCursorFlipbook() const {
	if (CursorFlipbook && CursorImage) {
		CursorImage->SetFlipbook(CursorFlipbook);
	}
}
