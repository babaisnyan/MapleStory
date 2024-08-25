#include "Actors/Monster.h"

#include "Components/MsSpriteComponent.h"
#include "Data/Table/MobTemplate.h"
#include "Managers/MobManager.h"

AMonster::AMonster() {
	PrimaryActorTick.bCanEverTick = true;
}

void AMonster::Init(const int32 Id) {
	MobId = Id;

	const UMobManager* MobManager = GetGameInstance()->GetSubsystem<UMobManager>();

	if (!MobManager) {
		return;
	}

	const FMobTemplate* MobTemplate = MobManager->GetMobTemplate(Id);

	if (!MobTemplate) {
		return;
	}

	Setup(MobTemplate);

	check(MobTemplate->HasStand);
	AddAnimation(EMobActionType::Stand, TEXT("stand"));

	if (MobTemplate->HasMove) {
		AddAnimation(EMobActionType::Move, TEXT("move"));
	}

	if (MobTemplate->HasDie) {
		AddAnimation(EMobActionType::Die, TEXT("die1"));
	}

	if (MobTemplate->HasHit) {
		AddAnimation(EMobActionType::Hit, TEXT("hit1"));
	}

	if (MobTemplate->HasRegen) {
		AddAnimation(EMobActionType::Regen, TEXT("regen"));
	}
}

void AMonster::SetCurrentAction(const EMobActionType ActionType) {
	if (CurrentAction == ActionType) {
		return;
	}

	if (SpriteComponents.Contains(CurrentAction)) {
		SpriteComponents[CurrentAction]->Reset();
		SpriteComponents[CurrentAction]->SetVisibility(false, true);
	}

	CurrentAction = ActionType;

	if (SpriteComponents.Contains(CurrentAction)) {
		SpriteComponents[CurrentAction]->Reset();
		SpriteComponents[CurrentAction]->Play();
		SpriteComponents[CurrentAction]->SetVisibility(true, true);
	}
}

void AMonster::BeginPlay() {
	Super::BeginPlay();

	check(SpriteComponents.Contains(EMobActionType::Stand));
	SpriteComponents[EMobActionType::Stand]->Play();
}


void AMonster::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if (Hp <= 0) {
		SetCurrentAction(EMobActionType::Die);
	}
}

void AMonster::Setup(const FMobTemplate* MobTemplate) {
	MobName = MobTemplate->MobName;
	Level = MobTemplate->Level;
	Hp = MobTemplate->MaxHp;
	BodyAttack = MobTemplate->BodyAttack;
	FirstAttack = MobTemplate->FirstAttack;
	Speed = MobTemplate->Speed;
	PaDamage = MobTemplate->PaDamage;
	MaDamage = MobTemplate->MaDamage;
	PdDamage = MobTemplate->PdDamage;
	MdDamage = MobTemplate->MdDamage;
	PdRate = MobTemplate->PdRate;
	MdRate = MobTemplate->MdRate;
	Exp = MobTemplate->Exp;
}

void AMonster::AddAnimation(const EMobActionType ActionType, const FString& ActionName) {
	const UDataTable* SpriteTable = LoadObject<UDataTable>(nullptr, *FString::Printf(TEXT("/Game/Mob/%d/%s/DT_Frames.DT_Frames"), MobId, *ActionName));

	if (!SpriteTable) {
		return;
	}

	UMsSpriteComponent* SpriteComponent = NewObject<UMsSpriteComponent>(this);
	SpriteComponent->Setup(SpriteTable, false, ActionType == EMobActionType::Stand || ActionType == EMobActionType::Move);
	SpriteComponents.Add(ActionType, SpriteComponent);
}
