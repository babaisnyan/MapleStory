// Fill out your copyright notice in the Description page of Project Settings.


#include "MapleGameMode.h"

#include "Characters/MaplePlayerController.h"
#include "Characters/MsPlayer.h"


AMapleGameMode::AMapleGameMode() {
	PlayerControllerClass = AMaplePlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	
	ConstructorHelpers::FClassFinder<AMsPlayer> GoTalk(TEXT("/Game/Blueprints/Characters/BP_GoTalk.BP_GoTalk_C"));
	ConstructorHelpers::FClassFinder<AMsPlayer> Serena(TEXT("/Game/Blueprints/Characters/BP_Serena.BP_Serena_C"));
	ConstructorHelpers::FClassFinder<AMsPlayer> Luke(TEXT("/Game/Blueprints/Characters/BP_Luke.BP_Luke_C"));

	if (GoTalk.Succeeded()) {
		PlayerClasses.Emplace(EAvatarType::GoTalk, GoTalk.Class);
	}

	if (Serena.Succeeded()) {
		PlayerClasses.Emplace(EAvatarType::Serena, Serena.Class);
	}

	if (Luke.Succeeded()) {
		PlayerClasses.Emplace(EAvatarType::Luke, Luke.Class);
	}
}

void AMapleGameMode::BeginPlay() {
	Super::BeginPlay();

	
	
	
	if (!GetWorld()->GetMapName().Equals("MAP_Login") && PlayerClasses.Contains(AvatarType)) {
		GetWorld()->SpawnActor<AMsPlayer>(PlayerClasses[AvatarType], FVector::ZeroVector, FRotator::ZeroRotator);
	}
}
