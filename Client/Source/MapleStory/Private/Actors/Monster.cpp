#include "Actors/Monster.h"

#include "Components/BoxComponent.h"
#include "Components/MobController.h"
#include "Components/MobStatComponent.h"
#include "Components/MsSpriteComponent.h"
#include "Data/Table/MobTemplate.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/MobManager.h"

AMonster::AMonster() {
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AMobController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(TEXT("Monster"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(true);
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);

	RootComponent = BoxComponent;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(RootComponent);

	StatComponent = CreateDefaultSubobject<UMobStatComponent>(TEXT("StatComponent"));

	static int32 Z = 0;
	ZIndex = ++Z;
}

bool AMonster::Init(const int32 Id, const int64 ObjId, const float Y, const EMobActionType ActionType, const bool Flip) {
	MobId = Id;
	ObjectId = ObjId;
	CurrentAction = ActionType;
	bFlip = Flip;
	DestY = Y;

	// log Y
	UE_LOG(LogTemp, Warning, TEXT("Y: %f"), Y);

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

void AMonster::Move(const protocol::GameServerMobMove& Packet) {
	// bFlip = Packet.flip();
	// DestX = Packet.x();
	//
	// if (!bFlip) {
	// 	SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
	// } else {
	// 	SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	// }
	//
	// SetCurrentAction(static_cast<EMobActionType>(Packet.state()));

	// get ai controller
	AMobController* MobController = Cast<AMobController>(GetController());
	const FVector DestVector = {Packet.x() + BaseX, 0.0f, DestY};
	MobController->MoveToLocation(DestVector, -1, false);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Move to location: %f, %f"), DestVector.X, DestVector.Z));
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

	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		const FVector SpawnPointLocation = PlayerStart->GetActorLocation();
		BaseX = SpawnPointLocation.X;
		BaseY = SpawnPointLocation.Z;
	}

	if (!bFlip) {
		SetActorRotation({0, 180.0f, 0.0f});
	} else {
		SetActorRotation({0, 0, 0});
	}

	check(SpriteComponents.Contains(CurrentAction));
	SetCurrentAction(CurrentAction, true);
}


void AMonster::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	// const FVector DestVector = {DestX + BaseX, 0.0f, DestY};
	//
	// if (DestVector != GetActorLocation()) {
	// 	const FVector TargetVector = FMath::VInterpTo(GetActorLocation(), DestVector, DeltaTime, 1.0f);
	// 	SetActorLocation(TargetVector);
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
	AttackWidth = MobTemplate->AttackWidth;
	AttackHeight = MobTemplate->AttackHeight;
}

void AMonster::OnCollisionUpdate(const FVector2D Size) {
	BoxComponent->SetBoxExtent(FVector(Size.X, 5.0f, Size.Y));
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
	SpriteComponent->TranslucencySortPriority = ZIndex;
	SpriteComponents.Add(ActionType, SpriteComponent);

	if (bRegisterEvent) {
		SpriteComponent->OnFinishedPlaying.AddDynamic(this, &AMonster::OnFinishedPlaying);
	}

	SpriteComponent->OnCollisionUpdate.AddDynamic(this, &AMonster::OnCollisionUpdate);
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
