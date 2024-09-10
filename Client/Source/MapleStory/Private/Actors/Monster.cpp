#include "Actors/Monster.h"

#include "Components/BoxComponent.h"
#include "Components/MobController.h"
#include "Components/MobStatComponent.h"
#include "Components/MsSpriteComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/Table/MobTemplate.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/MobManager.h"
#include "UI/MobNameTag.h"

AMonster::AMonster() {
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(TEXT("Monster"));
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnBeginOverlap);

	RootComponent = BoxComponent;

	static ConstructorHelpers::FClassFinder<UMobNameTag> NameTagFinder(TEXT("/Game/UI/Common/WBP_MobName.WBP_MobName_C"));
	if (NameTagFinder.Succeeded()) {
		NameTag = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTag"));
		NameTag->SetWidgetClass(NameTagFinder.Class);
		NameTag->SetWidgetSpace(EWidgetSpace::World);
		NameTag->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		NameTag->SetCollisionProfileName(TEXT("NoCollision"));
		NameTag->SetGenerateOverlapEvents(false);
		NameTag->SetSimulatePhysics(false);
		NameTag->CanCharacterStepUpOn = ECB_No;
		NameTag->SetBlendMode(EWidgetBlendMode::Transparent);
		NameTag->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NameTag->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NameTag->SetDrawAtDesiredSize(true);
		NameTag->TranslucencySortPriority = 100;
	}

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

	SetActorLabel(FString::Printf(TEXT("Monster_%d_%lld"), Id, ObjectId));

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
	const float Temp = DestX;
	bFlip = Packet.flip();
	DestX = Packet.x();

	if (abs(Temp - DestX) > 100) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Monster_%d_%lld Move Error"), MobId, ObjectId));
	}

	if (!bFlip) {
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		NameTag->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	} else {
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		NameTag->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	SetCurrentAction(static_cast<EMobActionType>(Packet.state()));
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
		NameTag->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	} else {
		SetActorRotation({0, 0, 0});
		NameTag->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	check(SpriteComponents.Contains(CurrentAction));
	SetCurrentAction(CurrentAction, true);

	if (NameTag) {
		if (const auto Widget = Cast<UMobNameTag>(NameTag->GetUserWidgetObject())) {
			Widget->SetInfo(Level, MobName);
		}
	}
}


void AMonster::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	const FVector DestVector = {DestX + BaseX, 0.0f, DestY};

	if (DestVector != GetActorLocation()) {
		const FVector TargetVector = FMath::VInterpTo(GetActorLocation(), DestVector, DeltaTime, 0.5f);
		SetActorLocation(TargetVector);
	}

	if (StatComponent->Hp <= 0) {
		SetCurrentAction(EMobActionType::Die);
	}

	if (NameTag && NameTag->GetDrawSize().X == 500) {
		if (const auto Widget = Cast<UMobNameTag>(NameTag->GetUserWidgetObject())) {
			const auto Size = Widget->GetDesiredSize();

			if (Size.X > 0 && Size.Y > 0) {
				const FVector BoxLocation = BoxComponent->GetComponentLocation();
				const FVector BoxExtent = BoxComponent->GetScaledBoxExtent();
				const FVector Location = BoxLocation - FVector(0.0f, 0.0f, BoxExtent.Z * 2);

				NameTag->SetDrawSize(Size);
				NameTag->SetWorldLocation(Location);
			}
		}
	}
}

void AMonster::Setup(const FMobTemplate* MobTemplate) {
	MobName = MobTemplate->MobName;
	Level = MobTemplate->Level;
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
