// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWindow.h"

#include "Kismet/GameplayStatics.h"
#include "MapleStory/MapleGameInstance.h"

void ULoginWindow::TryLogin() {
	const auto GameInstance = Cast<UMapleGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance) {
		RemoveFromParent();
		GameInstance->ChangeLoginState(ELoginState::InGame);
	}
}
