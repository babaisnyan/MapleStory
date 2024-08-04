#include "GameModes/MapleGameMode.h"

#include "MapleGameInstance.h"
#include "Characters/MaplePlayerController.h"
#include "Characters/MsLocalPlayer.h"
#include "GameFramework/GameUserSettings.h"


AMapleGameMode::AMapleGameMode() {
	PlayerControllerClass = AMaplePlayerController::StaticClass();
	DefaultPawnClass = nullptr;
}

void AMapleGameMode::BeginPlay() {
	Super::BeginPlay();
	const auto GameInstance = Cast<UMapleGameInstance>(GetGameInstance());

	GameInstance->CurrentPlayer = GetWorld()->SpawnActorDeferred<AMsLocalPlayer>(AMsLocalPlayer::StaticClass(), FTransform::Identity);
	GameInstance->CurrentPlayer->Setup(GameInstance->PlayerInfoTemp.GetValue());
	GameInstance->CurrentPlayer->FinishSpawning(FTransform::Identity);
	GameInstance->PlayerInfoTemp.Reset();

	const FIntPoint NewResolution(1920, 1080);
	GEngine->GameUserSettings->SetScreenResolution(NewResolution);
	GEngine->GameUserSettings->ApplyResolutionSettings(false);
}

void AMapleGameMode::AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo) {
	// const auto OtherPlayer = GetWorld()->SpawnActorDeferred<AMsPlayer>(PlayerClasses[static_cast<EAvatarType>(OtherPlayerInfo.type())], FTransform::Identity);
	// OtherPlayer->Setup(OtherPlayerInfo);
	// OtherPlayer->FinishSpawning(FTransform::Identity);
	// OtherPlayers.Emplace(OtherPlayerInfo.id(), OtherPlayer);
}
