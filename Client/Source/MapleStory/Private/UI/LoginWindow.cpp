// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoginWindow.h"

#include "MapleGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULoginWindow::TryLogin() {
	const auto GameInstance = Cast<UMapleGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance) {
		RemoveFromParent();
		GameInstance->ChangeLoginState(ELoginState::InGame);
	}
}
