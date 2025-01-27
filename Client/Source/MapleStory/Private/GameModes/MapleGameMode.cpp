#include "GameModes/MapleGameMode.h"

#include "MapleGameInstance.h"
#include "game_protocol.pb.h"
#include "Actors/Monster.h"
#include "Actors/MsSpriteActor.h"
#include "Characters/MaplePlayerController.h"
#include "Characters/MsLocalPlayer.h"
#include "Data/Table/MobTemplate.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SoundManager.h"


AMapleGameMode::AMapleGameMode() {
	PlayerControllerClass = AMaplePlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	InitExpTables();
}

void AMapleGameMode::BeginPlay() {
	Super::BeginPlay();

	const auto GameInstance = Cast<UMapleGameInstance>(GetGameInstance());

	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		PlayerStartLocation = PlayerStart->GetActorLocation();
	}
	
	GameInstance->CurrentPlayer = GetWorld()->SpawnActorDeferred<AMsLocalPlayer>(AMsLocalPlayer::StaticClass(), FTransform::Identity);
	GameInstance->CurrentPlayer->Setup(GameInstance->PlayerInfoTemp.GetValue());
	GameInstance->CurrentPlayer->FinishSpawning(FTransform::Identity);
	Players.Add(GameInstance->PlayerInfoTemp.GetValue().object_id(), GameInstance->CurrentPlayer);
	GameInstance->PlayerInfoTemp.Reset();

	while (!GameInstance->OtherPlayersQueue.IsEmpty()) {
		TOptional<protocol::OtherPlayerInfo> OtherPlayerInfo;
		GameInstance->OtherPlayersQueue.Dequeue(OtherPlayerInfo);

		if (OtherPlayerInfo.IsSet()) {
			AddPlayer(OtherPlayerInfo.GetValue());
		}
	}

	while (!GameInstance->RemovePlayerQueue.IsEmpty()) {
		int32 PlayerId;
		GameInstance->RemovePlayerQueue.Dequeue(PlayerId);
		RemovePlayer(PlayerId);
	}

	while (!GameInstance->MonstersQueue.IsEmpty()) {
		TOptional<protocol::MobInfo> MonsterInfo;
		GameInstance->MonstersQueue.Dequeue(MonsterInfo);
		AddMonster(MonsterInfo.GetValue());
	}

	// const FIntPoint NewResolution(1920, 1080);
	// GEngine->GameUserSettings->SetScreenResolution(NewResolution);
	// GEngine->GameUserSettings->ApplyResolutionSettings(false);
	GEngine->Exec(GetWorld(), TEXT("r.setRes 1280x720w"));
}

void AMapleGameMode::AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo) {
	const auto OtherPlayer = GetWorld()->SpawnActorDeferred<AMsPlayerBase>(AMsPlayerBase::StaticClass(), FTransform::Identity);
	OtherPlayer->Setup(OtherPlayerInfo);
	OtherPlayer->FinishSpawning(FTransform::Identity);
	Players.Add(OtherPlayerInfo.object_id(), OtherPlayer);
}

void AMapleGameMode::RemovePlayer(const int64 ObjectId) {
	if (Players.Contains(ObjectId)) {
		Players[ObjectId]->Destroy();
		Players.Remove(ObjectId);
	}
}

void AMapleGameMode::UpdatePlayerPosition(const protocol::GameServerPlayerMove& MovePacket) {
	if (Players.Contains(MovePacket.object_id())) {
		Players[MovePacket.object_id()]->Move(MovePacket);
	}
}

void AMapleGameMode::AddMonster(const protocol::MobInfo& MonsterInfo) {
	const auto Mob = GetWorld()->SpawnActorDeferred<AMonster>(AMonster::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!Mob || !Mob->Init(MonsterInfo, PlayerStartLocation.Z + MonsterInfo.y())) {
		return;
	}

	Mob->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(PlayerStartLocation.X + MonsterInfo.x(), 1, PlayerStartLocation.Z + MonsterInfo.y())));
	Monsters.Add(MonsterInfo.object_id(), Mob);
}

void AMapleGameMode::RemoveMonster(const int64 ObjectId) {
	if (Monsters.Contains(ObjectId)) {
		Monsters[ObjectId]->Destroy();
		Monsters.Remove(ObjectId);
	}
}

void AMapleGameMode::UpdateMonsterPosition(const protocol::GameServerMobMove& Packet) {
	if (Monsters.Contains(Packet.object_id())) {
		Monsters[Packet.object_id()]->Move(Packet);
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Monster not found"));
	}
}

void AMapleGameMode::PlayAttackAnimation(const protocol::GameServerMobAttack& Packet) {
	if (!Monsters.Contains(Packet.mob_id())) {
		return;
	}

	if (!Players.Contains(Packet.target_id())) {
		return;
	}

	Monsters[Packet.mob_id()]->SetCurrentAction(EMobActionType::Attack, true);

	const auto* Template = Monsters[Packet.mob_id()]->GetMobTemplate();
	const FVector Location = Players[Packet.target_id()]->GetActorLocation();
	const FVector NewLocation = FVector(Location.X, Location.Y + 10, Location.Z + 5);
	AMsSpriteActor* Sprite = GetWorld()->SpawnActorDeferred<AMsSpriteActor>(AMsSpriteActor::StaticClass(), FTransform::Identity, this);
	const UDataTable* SpriteTable = LoadObject<UDataTable>(nullptr, *FString::Printf(TEXT("/Game/Mob/%d/hit/DT_Frames.DT_Frames"), Template->MobId));

	if (!SpriteTable) {
		return;
	}

	Sprite->Setup(SpriteTable, Template->AttackDelay);
	Sprite->SetPriority(10000000);
	Sprite->FinishSpawning(FTransform(NewLocation));
	Sprite->AttachToActor(Players[Packet.target_id()], FAttachmentTransformRules::KeepRelativeTransform);

	const USoundManager* SoundManager = GetGameInstance()->GetSubsystem<USoundManager>();

	if (!SoundManager) {
		return;
	}

	if (Template->AttackSound.IsNull()) {
		return;
	}

	SoundManager->PlaySoundEffect(Template->AttackSound);

	if (Players[Packet.target_id()]->bIsInvincible) {
		return;
	}

	if (!Players[Packet.target_id()]->bIsLocalPlayer) {
		return;
	}

	if (Template->CharHitSound.IsNull()) {
		return;
	}

	FTimerHandle TimerHandle;
	const float Delay = Template->AttackDelay * 0.9f / 1000.0f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&Template, &SoundManager] {
		SoundManager->PlaySoundEffect(Template->CharHitSound);
	}), Delay, false);
}

uint64_t AMapleGameMode::GetExpForLevel(const int32 Level) const {
	if (Level < 0 || Level >= 300) {
		return 1;
	}

	return ExpTable[Level];
}

void AMapleGameMode::InitExpTables() {
	ExpTable[0] = 0;
	ExpTable[1] = 15;
	ExpTable[2] = 34;
	ExpTable[3] = 57;
	ExpTable[4] = 92;
	ExpTable[5] = 135;
	ExpTable[6] = 372;
	ExpTable[7] = 560;
	ExpTable[8] = 840;
	ExpTable[9] = 1242;

	for (int i = 10; i <= 14; i++) {
		ExpTable[i] = ExpTable[i - 1];
	}

	for (int i = 15; i <= 29; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.2f;
	}

	for (int i = 30; i <= 34; i++) {
		ExpTable[i] = ExpTable[i - 1];
	}

	for (int i = 35; i <= 39; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.2f;
	}

	for (int i = 40; i <= 59; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.08f;
	}

	for (int i = 60; i <= 64; i++) {
		ExpTable[i] = ExpTable[i - 1];
	}

	for (int i = 65; i <= 74; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.075f;
	}

	for (int i = 75; i <= 89; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.07f;
	}

	for (int i = 90; i <= 99; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.065f;
	}

	for (int i = 100; i <= 104; i++) {
		ExpTable[i] = ExpTable[i - 1];
	}

	for (int i = 105; i <= 139; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.065f;
	}

	for (int i = 140; i <= 169; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.0625f;
	}

	for (int i = 170; i <= 199; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.05f;
	}

	ExpTable[200] = 2207026470;

	for (int i = 201; i <= 209; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.12f;
	}

	ExpTable[210] = ExpTable[209] * 1.3f;

	for (int i = 211; i <= 214; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.11f;
	}

	ExpTable[215] = ExpTable[214] * 1.3f;

	for (int i = 216; i <= 219; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.09f;
	}

	ExpTable[220] = ExpTable[219] * 1.3f;

	for (int i = 221; i <= 224; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.07f;
	}

	ExpTable[225] = ExpTable[224] * 1.3f;

	for (int i = 226; i <= 229; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.07f;
	}

	ExpTable[230] = ExpTable[229] * 1.3f;

	for (int i = 231; i <= 234; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.03f;
	}

	ExpTable[235] = ExpTable[234] * 1.3f;

	for (int i = 236; i <= 239; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.03f;
	}

	ExpTable[240] = ExpTable[239] * 1.3f;

	for (int i = 241; i <= 244; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.03f;
	}

	ExpTable[245] = ExpTable[244] * 1.3f;

	for (int i = 246; i <= 249; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.03f;
	}

	ExpTable[250] = ExpTable[249] * 1.5f;

	for (int i = 251; i <= 259; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.03f;
	}

	ExpTable[260] = ExpTable[259] * 3;

	for (int i = 261; i <= 264; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.01f;
	}

	ExpTable[265] = ExpTable[264] * 1.3f;

	for (int i = 266; i <= 269; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.01f;
	}

	ExpTable[270] = ExpTable[269] * 2.22f;

	for (int i = 271; i <= 274; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.01f;
	}

	ExpTable[275] = ExpTable[274] * 2.02f;

	for (int i = 276; i <= 279; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.1f;
	}

	ExpTable[280] = ExpTable[279] * 2.02f;

	for (int i = 281; i <= 284; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.1f;
	}

	ExpTable[285] = ExpTable[284] * 2.02f;

	for (int i = 286; i <= 289; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.1f;
	}

	ExpTable[290] = ExpTable[289] * 2.02f;

	for (int i = 291; i <= 294; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.1f;
	}

	ExpTable[290] = ExpTable[289] * 2.02f;

	for (int i = 296; i <= 298; i++) {
		ExpTable[i] = ExpTable[i - 1] * 1.1f;
	}

	ExpTable[299] = ExpTable[298] * 1.5f;
}
