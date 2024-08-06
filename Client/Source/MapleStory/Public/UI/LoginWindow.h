#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "LoginWindow.generated.h"

class UEditableTextBox;

UCLASS()
class MAPLESTORY_API ULoginWindow : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool TryLogin();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> UsernameTextBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> PasswordTextBox;
};
