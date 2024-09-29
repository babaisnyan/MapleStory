#pragma once

#include "CoreMinimal.h"
#include "game_enum.pb.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

class UOverlay;
class UChatHistory;
class UScrollBox;
class UEditableTextBox;

UCLASS()
class MAPLESTORY_API UChatWidget : public UUserWidget {
	GENERATED_BODY()

public:
	void AddChat(const FString& InText, protocol::ChatType ChatType = protocol::CHAT_TYPE_NORMAL) const;

	UFUNCTION(BlueprintCallable)
	void ToggleChat();

protected:
	UFUNCTION(BlueprintCallable)
	void SendChat(const FString& InText);

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatHistory> ChatHistoryClass;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> HistoryPanel;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatHistoryBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UEditableTextBox> ChatTextBox;
};
