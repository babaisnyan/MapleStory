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

	while (!GameInstance->OtherPlayersQueue.IsEmpty()) {
		protocol::OtherPlayerInfo OtherPlayerInfo;
		GameInstance->OtherPlayersQueue.Dequeue(OtherPlayerInfo);
		AddPlayer(OtherPlayerInfo);
	}

	while (!GameInstance->RemovePlayerQueue.IsEmpty()) {
		int32 PlayerId;
		GameInstance->RemovePlayerQueue.Dequeue(PlayerId);
		RemovePlayer(PlayerId);
	}

	// const FIntPoint NewResolution(1920, 1080);
	// GEngine->GameUserSettings->SetScreenResolution(NewResolution);
	// GEngine->GameUserSettings->ApplyResolutionSettings(false);
	GEngine->Exec(GetWorld(), TEXT("r.setRes 1920x1080w"));
}

void AMapleGameMode::AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo) {
	const auto OtherPlayer = GetWorld()->SpawnActorDeferred<AMsPlayerBase>(AMsPlayerBase::StaticClass(), FTransform::Identity);
	OtherPlayer->Setup(OtherPlayerInfo);
	OtherPlayer->FinishSpawning(FTransform::Identity);
	OtherPlayers.Emplace(OtherPlayerInfo.object_id(), OtherPlayer);
}

void AMapleGameMode::RemovePlayer(const int64 ObjectId) {
	if (OtherPlayers.Contains(ObjectId)) {
		OtherPlayers[ObjectId]->Destroy();
		OtherPlayers.Remove(ObjectId);
	}
}
