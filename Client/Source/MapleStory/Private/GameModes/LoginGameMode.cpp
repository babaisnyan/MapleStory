// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/LoginGameMode.h"

#include "MapleGameInstance.h"
#include "Characters/LoginCharacter.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Network/LoginServerPacketHandler.h"
#include "Network/PacketCreator.h"
#include "UI/LoginMessageWindow.h"

void ALoginGameMode::BeginPlay() {
	Super::BeginPlay();

	const FIntPoint NewResolution(1280, 960);
	GEngine->GameUserSettings->SetScreenResolution(NewResolution);
	GEngine->GameUserSettings->ApplyResolutionSettings(false);

	const TObjectPtr<APawn> Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	const TObjectPtr<ALoginCharacter> LoginCharacter = Cast<ALoginCharacter>(Pawn);

	if (LoginCharacter) {
		LoginCharacterVisualizer = LoginCharacter;
	}

	WindowClass = StaticLoadClass(ULoginMessageWindow::StaticClass(), nullptr, TEXT("/Game/UI/Login/WB_LoginNotice.WB_LoginNotice_C"));
}

void ALoginGameMode::StartGame() {
	const UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	check(LoginCharacters.Num() > SelectedPlayerIndex);

	const auto SendBuffer = FPacketCreator::GetSelectCharacterRequest(LoginCharacters[SelectedPlayerIndex].id());
	GameInstance->SendPacket(SendBuffer);
}

void ALoginGameMode::Delete() {
	UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());
	if (!GameInstance) return;

	check(LoginCharacters.Num() > SelectedPlayerIndex);

	const auto SendBuffer = FPacketCreator::GetDeleteCharacterRequest(LoginCharacters[SelectedPlayerIndex].id());
	GameInstance->SendPacket(SendBuffer);
}

void ALoginGameMode::CreateCharacter(const FString Name, EAvatarType Avatar) {
	const UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());
	if (!GameInstance) return;
	
	if (Name.IsEmpty()) {
		ULoginMessageWindow* Window = CreateWidget<ULoginMessageWindow>(FLoginServerPacketHandler::GameInstance, WindowClass);
		Window->ErrorCode = 5;
		Window->AddToViewport();
		return;
	}

	if (Name.Len() > 8) {
		ULoginMessageWindow* Window = CreateWidget<ULoginMessageWindow>(FLoginServerPacketHandler::GameInstance, WindowClass);
		Window->ErrorCode = 5;
		Window->AddToViewport();
		return;
	}

	const auto SendBuffer = FPacketCreator::GetCreateCharacterRequest(Name, static_cast<int32>(Avatar));
	GameInstance->SendPacket(SendBuffer);
}

FString ALoginGameMode::GetSelectedPlayerName() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return "";
	}
	
	FString Name = UTF8_TO_TCHAR(LoginCharacters[SelectedPlayerIndex].name().c_str());
	return Name;
}

int32 ALoginGameMode::GetSelectedPlayerStr() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return 0;
	}
	
	return LoginCharacters[SelectedPlayerIndex].str();
}

int ALoginGameMode::GetSelectedPlayerDex() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return 0;
	}
	
	return LoginCharacters[SelectedPlayerIndex].dex();
}

int ALoginGameMode::GetSelectedPlayerInt() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return 0;
	}

	return LoginCharacters[SelectedPlayerIndex].int_();
}

int ALoginGameMode::GetSelectedPlayerLuk() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return 0;
	}

	return LoginCharacters[SelectedPlayerIndex].luk();
}

EJobType ALoginGameMode::GetSelectedPlayerJob() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return EJobType::Error;
	}

	return static_cast<EJobType>(LoginCharacters[SelectedPlayerIndex].job());
}

FString ALoginGameMode::GetSelectedPlayerLevelText() {
	if (SelectedPlayerIndex < 0 || SelectedPlayerIndex >= LoginCharacters.Num()) {
		return "";
	}

	FString LevelText = FString::Printf(TEXT("<img id=\"LoginLv\" width=\"desired\"/>"));
	const FString Level = FString::Printf(TEXT("%d"), LoginCharacters[SelectedPlayerIndex].level());

	for (int32 i = 0; i < Level.Len(); i++) {
		LevelText += FString::Printf(TEXT("<img id=\"LoginLv%d\" width=\"desired\"/>"), Level[i] - '0');
	}

	return LevelText;
}

void ALoginGameMode::DeleteCharacter(const int32 CharacterId) {
	for (int i = 0; i < LoginCharacters.Num(); i++) {
		if (LoginCharacters[i].id() == CharacterId) {
			LoginCharacterVisualizer->DeleteAvatar(i);
			LoginCharacters.RemoveAt(i);

			if (LoginCharacters.Num()) {
				SelectedPlayerIndex = 0;
			} else {
				SelectedPlayerIndex = -1;
				bCharacterSelected = false;
			}
			break;
		}
	}
}
