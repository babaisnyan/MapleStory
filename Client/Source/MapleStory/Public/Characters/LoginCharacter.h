#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "PaperCharacter.h"
#include "Data/Enum/EAvatarType.h"
#include "LoginCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MAPLESTORY_API ALoginCharacter : public APaperCharacter {
	GENERATED_BODY()

private:
	void ChangeSelection(const FInputActionValue& Value);
	void OnGameStartButton(const FInputActionValue& Value);

public:
	void SetAvatar(int32 Index, EAvatarType AvatarType, const FString& AvatarName);
	void SetLastAvatar(EAvatarType AvatarType, const FString& AvatarName);
	void DeleteAvatar(int32 Index);
	void ClearAvatars();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = Avatar)
	TArray<EAvatarType> Avatars = {EAvatarType::None, EAvatarType::None, EAvatarType::None, EAvatarType::None, EAvatarType::None, EAvatarType::None};

	UPROPERTY(BlueprintReadOnly, Category = Avatar)
	TArray<FString> AvatarNames = {FString(TEXT("")), FString(TEXT("")), FString(TEXT("")), FString(TEXT("")), FString(TEXT("")), FString(TEXT(""))};

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> CharSelectionAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> GameStartAction;

public:
	UPROPERTY(BlueprintReadOnly, Category = Avatar)
	int32 SelectedAvatarIndex = -1;

	UPROPERTY(BlueprintReadOnly, Category = Avatar)
	int32 MaxAvatarCount = 0;
};
