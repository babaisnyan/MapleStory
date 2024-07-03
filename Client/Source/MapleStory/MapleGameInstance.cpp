// Fill out your copyright notice in the Description page of Project Settings.

#include "MapleGameInstance.h"

#include "Kismet/GameplayStatics.h"


void UMapleGameInstance::ChangeLoginState(const ELoginState NewState) {
	LoginState = NewState;

	switch (NewState) {
		case ELoginState::Login:
			break;
		case ELoginState::CharacterSelection:
			break;
		case ELoginState::InGame:
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/MAP_Test"));
			break;
	}
}
