#include "Managers/MsCheatManager.h"

#include "EngineUtils.h"
#include "Actors/Monster.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void UMsCheatManager::SpawnMob(const int32 MobId) const {
	const auto PlayerCharacter = GetOuterAPlayerController()->GetCharacter();

	if (!PlayerCharacter) {
		return;
	}

	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const auto Mob = GetWorld()->SpawnActorDeferred<AMonster>(AMonster::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!Mob || !Mob->Init(MobId, 0)) {
		return;
	}

	Mob->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(PlayerLocation.X, 1, PlayerLocation.Z + 15)));
}

void UMsCheatManager::LogMobList() const {
	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (!PlayerStart) {
		return;
	}

	const FVector SpawnPointLocation = PlayerStart->GetActorLocation();

	for (TActorIterator<AMonster> It(GetWorld()); It; ++It) {
		const auto Mob = *It;
		const FVector Location = Mob->GetActorLocation();
		// get location which is on the floor
		const FVector NewLocation = {Location.X - SpawnPointLocation.X, Location.Y, Location.Z - SpawnPointLocation.Z + 3};

		UE_LOG(LogTemp, Warning, TEXT("MobId: %d, X: %f, Y: %f"), Mob->MobId, NewLocation.X, NewLocation.Z);
	}
}

void UMsCheatManager::MobSpawnTest() const {
	const auto PlayerCharacter = GetOuterAPlayerController()->GetCharacter();

	if (!PlayerCharacter) {
		return;
	}

	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (!PlayerStart) {
		return;
	}
	
	const FVector SpawnPointLocation = PlayerStart->GetActorLocation();

	constexpr int32 MobIds[] = {2300203, 2400610, 2400600, 2700310, 2510100, 2400500, 2400501, 2150003, 2150000, 2300201};
	constexpr float X[] = {-270.740783, -76.265564, 153.253113, 829.726807, 591.982667, 302.413086, 52.647399, -182.564941, -392.804138, -591.160371};
	constexpr float Y[] = {246.000000, 211.000000, 218.500000, -12.000000, -12.500000, -19.000000, -12.500000, -10.500000, -14.000000, -12.500000};

	for (int32 i = 0; i < 10; ++i) {
		const auto Mob = GetWorld()->SpawnActorDeferred<AMonster>(AMonster::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (!Mob || !Mob->Init(MobIds[i], 0)) {
			continue;
		}

		Mob->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(SpawnPointLocation.X + X[i], 1, SpawnPointLocation.Z + Y[i] - 3)));
	}
}
