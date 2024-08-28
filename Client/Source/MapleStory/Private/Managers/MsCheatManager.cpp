#include "Managers/MsCheatManager.h"

#include "Actors/Monster.h"
#include "GameFramework/Character.h"

void UMsCheatManager::SpawnMob(const int32 MobId) const {
	const auto PlayerCharacter = GetOuterAPlayerController()->GetCharacter();

	if (!PlayerCharacter) {
		return;
	}

	const FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	const auto Mob = GetWorld()->SpawnActorDeferred<AMonster>(AMonster::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if(!Mob || !Mob->Init(MobId)) {
		return;
	}
	
	Mob->FinishSpawning(FTransform(FRotator::ZeroRotator, FVector(PlayerLocation.X, PlayerLocation.Y - 1, PlayerLocation.Z + 10)));
}
