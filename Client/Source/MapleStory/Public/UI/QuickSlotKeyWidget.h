#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/Enum/EKeyCode.h"
#include "Data/Enum/EKeyType.h"
#include "QuickSlotKeyWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class MAPLESTORY_API UQuickSlotKeyWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UFUNCTION(BlueprintCallable)
	void LoadKeyTexture();

	UFUNCTION(BlueprintCallable)
	void LoadKeyCodeTexture();

private:
	UFUNCTION()
	void OnClicked();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKeyType KeyType = EKeyType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EKeyCode KeyCode = EKeyCode::Esc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SkillId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UTexture2D> KeyTexture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UTexture2D> KeyCodeTexture;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> KeyImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> KeyCodeImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCountText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> DummyButton;

private:
	const TMap<EKeyType, FString> KeyTexturePaths = {
		{EKeyType::CharInfo, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_CharInfo.T_KeyIcon_CharInfo'")},
		{EKeyType::Equipment, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Eqp.T_KeyIcon_Eqp'")},
		{EKeyType::Inventory, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Item.T_KeyIcon_Item'")},
		{EKeyType::SkillWindow, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Skill.T_KeyIcon_Skill'")},
		{EKeyType::Attack, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Attack.T_KeyIcon_Attack'")},
		{EKeyType::Jump, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Jump.T_KeyIcon_Jump'")},
		{EKeyType::KeyConfig, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Jump.T_KeyIcon_Jump'")},
		{EKeyType::Menu, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Menu.T_KeyIcon_Menu'")},
		{EKeyType::NpcTalk, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Npc.T_KeyIcon_Npc'")},
		{EKeyType::PickUp, TEXT("Texture2D'/Game/UI/Keys/Asset/T_KeyIcon_Pick.T_KeyIcon_Pick'")}
	};

	bool bInitialized = false;
};
