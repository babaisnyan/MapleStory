#include "Components/PlayerStatComponent.h"

UPlayerStatComponent::UPlayerStatComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerStatComponent::BeginPlay() {
	Super::BeginPlay();
}

void UPlayerStatComponent::Setup(const protocol::PlayerInfo& Info) {
	Level = Info.level();
	Hp = Info.hp();
	MaxHp = Info.max_hp();
	Mp = Info.mp();
	MaxMp = Info.max_mp();
	Str = Info.str();
	Dex = Info.dex();
	Int = Info.int_();
	Luk = Info.luk();
}
