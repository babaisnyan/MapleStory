#include "Actors/Monster.h"

#include "Components/BoxComponent.h"
#include "Components/MobStatComponent.h"
#include "Components/MsSpriteComponent.h"
#include "Data/Table/MobTemplate.h"
#include "Managers/MobManager.h"

AMonster::AMonster() {
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(TEXT("Monster"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(true);
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);

	RootComponent = BoxComponent;

	StatComponent = CreateDefaultSubobject<UMobStatComponent>(TEXT("StatComponent"));
}

bool AMonster::Init(const int32 Id) {
	MobId = Id;

	const UMobManager* MobManager = GetGameInstance()->GetSubsystem<UMobManager>();

	if (!MobManager) {
		return false;
	}

	const FMobTemplate* MobTemplate = MobManager->GetMobTemplate(Id);

	if (!MobTemplate) {
		return false;
	}

	Setup(MobTemplate);

	check(MobTemplate->HasStand);
	AddAnimation(EMobActionType::Stand, TEXT("stand"), false);

	if (MobTemplate->HasMove) {
		AddAnimation(EMobActionType::Move, TEXT("move"), false);
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

	if (MobTemplate->HasAttack) {
		AddAnimation(EMobActionType::Attack, TEXT("attack1"));
	}

	return true;
}

void AMonster::SetCurrentAction(const EMobActionType ActionType, const bool bForce) {
	if (CurrentAction == ActionType && !bForce) {
		return;
	}

	if (bForce) {
		for (const auto& Pair : SpriteComponents) {
			Pair.Value->Reset();
			Pair.Value->SetVisibility(false, true);
		}
	} else {
		if (SpriteComponents.Contains(CurrentAction)) {
			SpriteComponents[CurrentAction]->Reset();
			SpriteComponents[CurrentAction]->SetVisibility(false, true);
		}
	}

	CurrentAction = ActionType;

	if (SpriteComponents.Contains(CurrentAction)) {
		const TObjectPtr<UMsSpriteComponent> SpriteComponent = SpriteComponents[CurrentAction];
		SpriteComponent->Reset();
		SpriteComponent->Play();
		SpriteComponent->SetVisibility(true, true);
	}
}

void AMonster::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	#if WITH_EDITOR
	if (PropertyChangedEvent.Property) {
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();

		if (PropertyName == GET_MEMBER_NAME_CHECKED(AMonster, CurrentAction)) {
			SetCurrentAction(CurrentAction, true);
		}
	}
	#endif
}

void AMonster::BeginPlay() {
	Super::BeginPlay();

	check(SpriteComponents.Contains(EMobActionType::Stand));

	if (SpriteComponents.Contains(EMobActionType::Regen)) {
		SetCurrentAction(EMobActionType::Regen);
	} else {
		SetCurrentAction(EMobActionType::Stand);
	}
}


void AMonster::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if (SpriteComponents.Contains(CurrentAction)) {
		UMsSpriteComponent* SpriteComponent = SpriteComponents[CurrentAction];
		const FVector2D Size = SpriteComponent->GetSpriteSize();
		BoxComponent->SetBoxExtent(FVector(Size.X, 5.0f, Size.Y));
	}

	// if (AttackWidth > 0 && AttackHeight > 0) {
	// 	const FVector Location = GetActorLocation();
	// 	const FVector Start = Location + FVector(-AttackWidth / 2, 0, -AttackHeight / 2);
	// 	const FVector End = Location + FVector(AttackWidth / 2, 0, AttackHeight / 2);
	// 	DrawDebugBox(GetWorld(), Location, FVector(AttackWidth / 2, 0, AttackHeight / 2), FColor::Red, false, 0.1f, 0, 3);
	// }

	if (StatComponent->Hp <= 0) {
		SetCurrentAction(EMobActionType::Die);
	}
}

void AMonster::Setup(const FMobTemplate* MobTemplate) {
	MobName = MobTemplate->MobName;
	StatComponent->Level = MobTemplate->Level;
	StatComponent->Hp = MobTemplate->MaxHp;
	BodyAttack = MobTemplate->BodyAttack;
	FirstAttack = MobTemplate->FirstAttack;
	StatComponent->Speed = MobTemplate->Speed + 100 < 0 ? 0 : MobTemplate->Speed + 100;
	StatComponent->PhysicalAttack = MobTemplate->PaDamage;
	StatComponent->MagicalAttack = MobTemplate->MaDamage;
	StatComponent->PhysicalDefense = MobTemplate->PdDamage;
	StatComponent->MagicalDefense = MobTemplate->MdDamage;
	StatComponent->PhysicalDefenseRate = MobTemplate->PdRate;
	StatComponent->MagicalDefenseRate = MobTemplate->MdRate;
	Exp = MobTemplate->Exp;
	AttackCool = MobTemplate->AttackCool;
	AttackWidth = MobTemplate->AttackWidth;
	AttackHeight = MobTemplate->AttackHeight;
}

void AMonster::AddAnimation(const EMobActionType ActionType, const FString& ActionName, const bool bRegisterEvent) {
	const UDataTable* SpriteTable = LoadObject<UDataTable>(nullptr, *FString::Printf(TEXT("/Game/Mob/%d/%s/DT_Frames.DT_Frames"), MobId, *ActionName));

	if (!SpriteTable) {
		return;
	}

	UMsSpriteComponent* SpriteComponent = NewObject<UMsSpriteComponent>(this);
	SpriteComponent->Setup(SpriteTable, false, ActionType == EMobActionType::Stand || ActionType == EMobActionType::Move);
	SpriteComponent->RegisterComponent();
	SpriteComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpriteComponent->SetVisibility(false, true);
	SpriteComponents.Add(ActionType, SpriteComponent);

	if (bRegisterEvent) {
		SpriteComponent->OnFinishedPlaying.AddDynamic(this, &AMonster::OnFinishedPlaying);
	}
}

void AMonster::OnFinishedPlaying(UMsSpriteComponent* SpriteComponent) {
	if (!SpriteComponent) {
		return;
	}

	// TODO: 몹 ai 적용
	switch (CurrentAction) {
		case EMobActionType::Hit:
		case EMobActionType::Regen:
		case EMobActionType::Attack:
			SetCurrentAction(EMobActionType::Stand);
			break;
		case EMobActionType::Die:
			Destroy();
			break;
	}
}

void AMonster::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (CurrentAction == EMobActionType::Die || CurrentAction == EMobActionType::Regen) {
		return;
	}
}
