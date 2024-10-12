#include "Actors/Monster.h"

#include "Actors/DamageTextActor.h"
#include "Characters/MsLocalPlayer.h"
#include "Components/BoxComponent.h"
#include "Components/MobStatComponent.h"
#include "Components/MsSpriteComponent.h"
#include "Components/WidgetComponent.h"
#include "Data/Table/MobTemplate.h"
#include "GameFramework/PlayerStart.h"
#include "GameModes/MapleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/MobManager.h"
#include "UI/MobHpBar.h"
#include "UI/MobNameTag.h"

class AMapleGameMode;

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
		NameTag->SetDrawAtDesiredSize(true);
		NameTag->TranslucencySortPriority = 100;
	}

	static ConstructorHelpers::FClassFinder<UMobHpBar> HpBarFinder(TEXT("/Game/UI/Common/WBP_MobHpBar.WBP_MobHpBar_C"));
	if (HpBarFinder.Succeeded()) {
		HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
		HpBar->SetWidgetClass(HpBarFinder.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		HpBar->SetCollisionProfileName(TEXT("NoCollision"));
		HpBar->SetGenerateOverlapEvents(false);
		HpBar->SetSimulatePhysics(false);
		HpBar->CanCharacterStepUpOn = ECB_No;
		HpBar->SetBlendMode(EWidgetBlendMode::Transparent);
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HpBar->SetDrawAtDesiredSize(true);
		HpBar->TranslucencySortPriority = 101;
	}

	StatComponent = CreateDefaultSubobject<UMobStatComponent>(TEXT("StatComponent"));

	static int32 Z = 0;
	ZIndex = ++Z;
}

bool AMonster::Init(const protocol::MobInfo& MonsterInfo, const float Y) {
	MobId = MonsterInfo.id();
	ObjectId = MonsterInfo.object_id();
	CurrentAction = static_cast<EMobActionType>(MonsterInfo.state());
	bFlip = MonsterInfo.flip();
	DestY = Y;

	if (MonsterInfo.has_target_x()) {
		DestX = MonsterInfo.target_x();
	}

	SetActorLabel(FString::Printf(TEXT("Monster_%d_%lld"), MobId, ObjectId));

	UMobManager* MobManager = GetGameInstance()->GetSubsystem<UMobManager>();

	if (!MobManager) {
		return false;
	}

	FMobTemplate* MobTemplate = MobManager->GetMobTemplate(MobId);

	if (!MobTemplate) {
		return false;
	}

	this->Template = MobTemplate;
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

	NameTag->AttachToComponent(SpriteComponents[EMobActionType::Stand], FAttachmentTransformRules::KeepRelativeTransform);
	NameTag->SetRelativeLocation({0.0f, 0.0f, -55.5f});
	HpBar->AttachToComponent(SpriteComponents[EMobActionType::Stand], FAttachmentTransformRules::KeepRelativeTransform);
	HpBar->SetRelativeLocation({0.0f, 0.0f, 55.5f});
	StatComponent->Hp = MonsterInfo.hp();

	return true;
}

void AMonster::SetCurrentAction(const EMobActionType ActionType, const bool bForce) {
	if (CurrentAction == ActionType && !bForce) {
		return;
	}

	if (bForce) {
		for (const auto& Pair : SpriteComponents) {
			Pair.Value->Reset();
			Pair.Value->SetVisibility(false, false);
		}
	} else {
		if (SpriteComponents.Contains(CurrentAction)) {
			SpriteComponents[CurrentAction]->Reset();
			SpriteComponents[CurrentAction]->SetVisibility(false, false);
		}
	}

	LastAction = CurrentAction;
	CurrentAction = ActionType;

	if (SpriteComponents.Contains(CurrentAction)) {
		const TObjectPtr<UMsSpriteComponent> SpriteComponent = SpriteComponents[CurrentAction];
		SpriteComponent->Reset();
		SpriteComponent->Play();
		SpriteComponent->SetVisibility(true, false);
	}
}

void AMonster::Move(const protocol::GameServerMobMove& Packet) {
	bFlip = Packet.flip();

	if (Packet.state() != protocol::MOB_ACTION_TYPE_HIT) {
		if (Packet.has_target_id()) {
			const auto GameMode = GetWorld()->GetAuthGameMode<AMapleGameMode>();
			check(GameMode);
			check(GameMode->Players.Contains(Packet.target_id()));

			AgroPlayer = GameMode->Players[Packet.target_id()];
			DestX = AgroPlayer->GetActorLocation().X - BaseX;
		} else if (Packet.has_target_x()) {
			DestX = Packet.target_x();
			SetActorLocation(FVector(Packet.x() + BaseX, 0.0f, DestY));
			AgroPlayer = nullptr;
		} else {
			AgroPlayer = nullptr;
		}
	} else {
		AgroPlayer = nullptr;
	}

	if (!bFlip) {
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		NameTag->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		HpBar->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	} else {
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		NameTag->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		HpBar->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	if (static_cast<EMobActionType>(Packet.state()) == EMobActionType::Attack) {
		SetCurrentAction(EMobActionType::Stand);
	} else {
		SetCurrentAction(static_cast<EMobActionType>(Packet.state()));
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

void AMonster::OnDamaged(const int32 Damage) {
	const FVector Location = GetActorLocation();
	const FVector NewLocation = FVector(Location.X, Location.Y + 1, Location.Z + 20.0f);
	ADamageTextActor* Text = GetWorld()->SpawnActorDeferred<ADamageTextActor>(ADamageTextActor::StaticClass(), FTransform(NewLocation), this);
	Text->SetDamage(Damage, false, false);
	Text->FinishSpawning(FTransform(NewLocation));

	StatComponent->Hp -= Damage;

	if (Damage > 0 && StatComponent->Hp > 0) {
		SetCurrentAction(EMobActionType::Hit, true);
	} else {
		SetCurrentAction(EMobActionType::Die, true);
	}

	if (HpBar) {
		if (const auto Widget = Cast<UMobHpBar>(HpBar->GetUserWidgetObject())) {
			Widget->CurrentHp = FMath::Max(0, StatComponent->Hp);
			Widget->UpdateHpBar();
		}
	}
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
		HpBar->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	} else {
		SetActorRotation({0, 0, 0});
		NameTag->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		HpBar->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	check(SpriteComponents.Contains(CurrentAction));
	SetCurrentAction(CurrentAction, true);

	if (NameTag) {
		if (const auto Widget = Cast<UMobNameTag>(NameTag->GetUserWidgetObject())) {
			Widget->SetInfo(Level, MobName);
		}
	}

	const float NameTagDelta = NameTag->GetComponentLocation().Z - BoxComponent->GetComponentLocation().Z;
	NameTag->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	NameTag->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	NameTag->SetRelativeLocation({0.0f, 0.0f, NameTagDelta});

	HpBar->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	HpBar->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	const auto Bar = Cast<UMobHpBar>(HpBar->GetUserWidgetObject());

	if (Bar) {
		Bar->MaxHp = StatComponent->MaxHp;
		Bar->CurrentHp = StatComponent->Hp;
		Bar->UpdateHpBar();
	}
}


void AMonster::Tick(const float DeltaTime) {
	Super::Tick(DeltaTime);

	if (CurrentAction == EMobActionType::Move) {
		if (AgroPlayer) {
			const FVector PlayerLocation = AgroPlayer->GetActorLocation();
			bFlip = PlayerLocation.X < GetActorLocation().X;
		}

		const FVector DestLocation = FVector(GetActorLocation().X + StatComponent->Speed * DeltaTime * (bFlip ? -1.0f : 1.0f), 0.0f, DestY);
		const FVector EndLocation = FVector(DestLocation.X, DestLocation.Y, DestLocation.Z - 100.0f);
		FHitResult HitResult;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

		if (GetWorld()->LineTraceSingleByObjectType(HitResult, DestLocation, EndLocation, ObjectQueryParams)) {
			AddActorWorldOffset({StatComponent->Speed * DeltaTime * (bFlip ? -1.0f : 1.0f), 0.0f, 0.0f});
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Monster_%d_%lld can't find the ground"), MobId, ObjectId);
		}
	}

	if (HpBar && HpBar->GetDrawSize().X == 500) {
		if (const auto Widget = Cast<UMobHpBar>(HpBar->GetUserWidgetObject())) {
			const auto Size = Widget->GetDesiredSize();

			if (Size.X > 0 && Size.Y > 0) {
				HpBar->SetDrawSize(Size);
			}
		}
	}

	if (!bFlip) {
		SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
		NameTag->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		HpBar->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	} else {
		SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
		NameTag->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		HpBar->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	}

	if (StatComponent->Hp <= 0) {
		SetCurrentAction(EMobActionType::Die);
	}
}

void AMonster::Setup(const FMobTemplate* MobTemplate) {
	MobName = MobTemplate->MobName;
	Level = MobTemplate->Level;
	StatComponent->Level = MobTemplate->Level;
	StatComponent->Hp = MobTemplate->MaxHp;
	StatComponent->MaxHp = MobTemplate->MaxHp;
	BodyAttack = MobTemplate->BodyAttack;
	FirstAttack = MobTemplate->FirstAttack;
	StatComponent->Speed = (1.0f + static_cast<float>(MobTemplate->Speed) / 100.0f) * 70.0f;
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
	SpriteComponent->SetVisibility(false, false);
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

	// log last action and current action. print enum name
	UE_LOG(LogTemp, Warning, TEXT("Monster_%d_%lld Finished playing %s"), MobId, ObjectId, *UEnum::GetValueAsString(CurrentAction));

	switch (CurrentAction) {
		case EMobActionType::Hit:
		case EMobActionType::Regen:
		case EMobActionType::Attack:
			SetCurrentAction(EMobActionType::Stand, true);
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

	if (const auto Player = Cast<AMsLocalPlayer>(OtherActor)) {}
}
