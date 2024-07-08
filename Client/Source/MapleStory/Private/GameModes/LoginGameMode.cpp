// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LoginGameMode.h"

#include "Characters/LoginCharacter.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"

void ALoginGameMode::BeginPlay() {
	Super::BeginPlay();

	const FIntPoint NewResolution(1280, 960);
	GEngine->GameUserSettings->SetScreenResolution(NewResolution);
	GEngine->GameUserSettings->ApplyResolutionSettings(false);

	const TObjectPtr<APawn> Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	const TObjectPtr<ALoginCharacter> LoginCharacter = Cast<ALoginCharacter>(Pawn);

	if (LoginCharacter) {
		LoginCharacterVisualizer = LoginCharacter;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("LoginCharacterVisualizer is set"));
	}
}

void ALoginGameMode::StartGame() {
	
}
