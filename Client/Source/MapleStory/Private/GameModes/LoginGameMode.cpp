// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LoginGameMode.h"

#include "MapleGameInstance.h"
#include "Characters/LoginCharacter.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/PacketCreator.h"

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
	UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	check(LoginCharacters.Num() > SelectedPlayerIndex);

	const auto SendBuffer = FPacketCreator::GetSelectCharacterRequest(LoginCharacters[SelectedPlayerIndex].id());
	FLoginServerPacketHandler::GameInstance->SendPacket(SendBuffer);
}
