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
	FString GetSelectedPlayerName() {
		check(SelectedPlayerIndex >= 0 && SelectedPlayerIndex < LoginCharacters.Num());
		FString Name = UTF8_TO_TCHAR(LoginCharacters[SelectedPlayerIndex].name().c_str());
		return Name;
	}

	UFUNCTION(BlueprintCallable)
	int32 GetSelectedPlayerStr() {
		check(SelectedPlayerIndex >= 0 && SelectedPlayerIndex < LoginCharacters.Num());
		return LoginCharacters[SelectedPlayerIndex].str();
	}

	UFUNCTION(BlueprintCallable)
	int GetSelectedPlayerDex() {
		check(SelectedPlayerIndex >= 0 && SelectedPlayerIndex < LoginCharacters.Num());
		return LoginCharacters[SelectedPlayerIndex].dex();
	}

	UFUNCTION(BlueprintCallable)
	int GetSelectedPlayerInt() {
		check(SelectedPlayerIndex >= 0 && SelectedPlayerIndex < LoginCharacters.Num());
		return LoginCharacters[SelectedPlayerIndex].int_();
	}

	UFUNCTION(BlueprintCallable)
	int GetSelectedPlayerLuk() {
		check(SelectedPlayerIndex >= 0 && SelectedPlayerIndex < LoginCharacters.Num());
		return LoginCharacters[SelectedPlayerIndex].luk();
	}

	UFUNCTION(BlueprintCallable)
	EJobType GetSelectedPlayerJob() {
		check(SelectedPlayerIndex >= 0 && SelectedPlayerIndex < LoginCharacters.Num());
		return static_cast<EJobType>(LoginCharacters[SelectedPlayerIndex].job());
	}

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
