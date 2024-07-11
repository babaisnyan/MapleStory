#pragma once

#include "CoreMinimal.h"
#include "login_struct.pb.h"
#include "Data/Enum/EJobType.h"
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

	UFUNCTION(BlueprintCallable)
	void Delete();

	UFUNCTION(BlueprintCallable)
	FString GetSelectedPlayerName();

	UFUNCTION(BlueprintCallable)
	int32 GetSelectedPlayerStr();

	UFUNCTION(BlueprintCallable)
	int GetSelectedPlayerDex();

	UFUNCTION(BlueprintCallable)
	int GetSelectedPlayerInt();

	UFUNCTION(BlueprintCallable)
	int GetSelectedPlayerLuk();

	UFUNCTION(BlueprintCallable)
	EJobType GetSelectedPlayerJob();

	UFUNCTION(BlueprintCallable)
	FString GetSelectedPlayerLevelText();

	void DeleteCharacter(int32 CharacterId);
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bCharacterSelected = false;

	UPROPERTY(BlueprintReadOnly)
	bool bCanCreateCharacter = true;

	UPROPERTY(BlueprintReadOnly)
	int32 SelectedPlayerIndex = -1;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ALoginCharacter> LoginCharacterVisualizer = nullptr;

	TArray<protocol::LoginCharacter> LoginCharacters;
};
