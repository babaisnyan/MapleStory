#pragma once

#include "CoreMinimal.h"
#include "login_struct.pb.h"
#include "GameFramework/GameModeBase.h"
#include "LoginGameMode.generated.h"

class ALoginCharacter;

UCLASS()
class MAPLESTORY_API ALoginGameMode : public AGameModeBase {
	GENERATED_BODY()

private:
	virtual ~ALoginGameMode() override {
		LoginCharacters.Empty();
	}

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartGame();

public:
	UPROPERTY(BlueprintReadOnly)
	bool bCharacterSelected = true;

	UPROPERTY(BlueprintReadOnly)
	bool bCanCreateCharacter = true;

	UPROPERTY(BlueprintReadOnly)
	int32 SelectedPlayerIndex = -1;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ALoginCharacter> LoginCharacterVisualizer = nullptr;

	TArray<protocol::LoginCharacter> LoginCharacters;
};
