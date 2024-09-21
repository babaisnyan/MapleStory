#include "Characters/MsPlayerBase.h"

#include "PaperFlipbookComponent.h"
#include "PoolManagerSubsystem.h"
#include "PoolManagerTypes.h"
#include "Actors/DamageTextActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/PlayerStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Data/Table/AvatarData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/NameTag.h"

AMsPlayerBase::AMsPlayerBase() {
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	static ConstructorHelpers::FObjectFinder<UDataTable> AvatarTableFinder(TEXT("/Script/Engine.DataTable'/Game/Data/DT_Avatar.DT_Avatar'"));
	if (AvatarTableFinder.Succeeded()) {
		AvatarTable = AvatarTableFinder.Object;
	}

	const TObjectPtr<UCapsuleComponent> Capsule = GetCapsuleComponent();
	if (Capsule) {
		Capsule->SetCapsuleSize(16.0f, 33.0f);
		Capsule->SetCollisionProfileName(TEXT("Pawn"));
		Capsule->SetSimulatePhysics(false);
		Capsule->SetEnableGravity(true);
		Capsule->CanCharacterStepUpOn = ECB_No;
		Capsule->BodyInstance.bLockXRotation = true;
		Capsule->BodyInstance.bLockYRotation = true;
		RootComponent = Capsule;
	}

	static ConstructorHelpers::FClassFinder<UNameTag> NameTagFinder(TEXT("/Game/UI/Common/WBP_NameTag.WBP_NameTag_C"));
	if (NameTagFinder.Succeeded()) {
		NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTag"));
		NameTagWidget->SetWidgetClass(NameTagFinder.Class);
		NameTagWidget->SetWidgetSpace(EWidgetSpace::World);
		NameTagWidget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		NameTagWidget->SetRelativeLocation(FVector(0.0f, 10.0f, -50.0f));
		NameTagWidget->SetCollisionProfileName(TEXT("NoCollision"));
		NameTagWidget->SetGenerateOverlapEvents(false);
		NameTagWidget->SetSimulatePhysics(false);
		NameTagWidget->CanCharacterStepUpOn = ECB_No;
		NameTagWidget->SetBlendMode(EWidgetBlendMode::Transparent);
		NameTagWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NameTagWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		NameTagWidget->TranslucencySortPriority = 1000;
	}

	const TObjectPtr<UCharacterMovementComponent> Movement = GetCharacterMovement();
	if (Movement) {
		Movement->GravityScale = 1.5f;
		Movement->MaxWalkSpeed = 180.0f;
		Movement->JumpZVelocity = 500.0f;
	}

	GetSprite()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetSprite()->SetGenerateOverlapEvents(false);
	GetSprite()->CanCharacterStepUpOn = ECB_No;
	GetSprite()->SetRelativeLocation(FVector(0.0f, 1.0f, 0.0f));
	GetSprite()->SetFlipbook(IdleAnimation);

	PlayerStat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
}

void AMsPlayerBase::BeginPlay() {
	Super::BeginPlay();

	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		const FVector SpawnPointLocation = PlayerStart->GetActorLocation();
		BaseX = SpawnPointLocation.X;
		BaseY = SpawnPointLocation.Z;
		const FVector SpawnLocation = FVector(BaseX + StartX, SpawnPointLocation.Y, BaseY + StartY);
		SetActorLocation(SpawnLocation);
	}

	if (NameTagWidget) {
		if (const auto Widget = Cast<UNameTag>(NameTagWidget->GetUserWidgetObject())) {
			Widget->NameTagText->SetText(FText::FromString(Name));
		}
	}
}

void AMsPlayerBase::Setup(const protocol::PlayerInfo& Info) {
	PlayerStat->Setup(Info);
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
	StartX = Info.x();
	StartY = Info.y();
	bIsLocalPlayer = true;
	InitAnimation();
}

void AMsPlayerBase::Setup(const protocol::OtherPlayerInfo& Info) {
	PlayerStat->Setup(Info);
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
	StartX = Info.x();
	StartY = Info.y();
	bIsLocalPlayer = false;
	InitAnimation();
}

void AMsPlayerBase::Move(const protocol::GameServerPlayerMove& MovePacket) {
	bFlip = MovePacket.flip();
	AnimationType = MovePacket.animation();
	DestX = MovePacket.x();
	DestY = MovePacket.y();

	if (!bFlip) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	} else {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}

	UpdateAnimation();
}

void AMsPlayerBase::OnDamaged(const int32 Damage) {
	const FVector Location = GetActorLocation();
	const FVector NewLocation = FVector(Location.X, Location.Y + 1, Location.Z + 20.0f);
	ADamageTextActor* Text = GetWorld()->SpawnActorDeferred<ADamageTextActor>(ADamageTextActor::StaticClass(), FTransform(NewLocation), this);
	Text->SetDamage(Damage, true, false);
	Text->FinishSpawning(FTransform(NewLocation));
}

void AMsPlayerBase::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	if (NameTagWidget && NameTagWidget->GetDrawSize().X == 500) {
		if (const auto Widget = Cast<UNameTag>(NameTagWidget->GetUserWidgetObject())) {
			const auto Size = Widget->GetDesiredSize();

			if (Size.X > 0 && Size.Y > 0) {
				NameTagWidget->SetDrawSize(Size);
			}
		}
	}

	if (!bIsLocalPlayer) {
		const FVector DestVector = {DestX + BaseX, 0.0f, DestY + BaseY};

		if (DestVector != GetActorLocation()) {
			const FVector TargetVector = FMath::VInterpTo(GetActorLocation(), DestVector, DeltaSeconds, 50.0f);
			SetActorLocation(TargetVector);
		}
	}
}

void AMsPlayerBase::UpdatePosition() {
	const FRotator SpriteRotation = GetSprite()->GetRelativeRotation();
	bFlip = SpriteRotation.Yaw != 0.0f;
}

void AMsPlayerBase::InitAnimation() {
	const FAvatarData* AvatarData = AvatarTable->FindRow<FAvatarData>(FName(*FString::FromInt(static_cast<int32>(AvatarType))), TEXT(""));

	if (AvatarData) {
		IdleAnimation = AvatarData->IdleAnimation;
		RunAnimation = AvatarData->WalkAnimation;
		JumpAnimation = AvatarData->JumpAnimation;

		GetSprite()->SetFlipbook(IdleAnimation);
	}
}

void AMsPlayerBase::UpdateAnimation() const {
	switch (AnimationType) {
		case protocol::PLAYER_ANIMATION_RUN:
			GetSprite()->SetFlipbook(RunAnimation);
			break;
		case protocol::PLAYER_ANIMATION_JUMP:
			GetSprite()->SetFlipbook(JumpAnimation);
			break;
		default:
			GetSprite()->SetFlipbook(IdleAnimation);
			break;
	}
}
