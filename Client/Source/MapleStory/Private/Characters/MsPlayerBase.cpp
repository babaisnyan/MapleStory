#include "Characters/MsPlayerBase.h"

#include "PaperFlipbookComponent.h"
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
	static ConstructorHelpers::FObjectFinder<UDataTable> AvatarTableFinder(TEXT("/Script/Engine.DataTable'/Game/Data/DT_Avatar.DT_Avatar'"));
	if (AvatarTableFinder.Succeeded()) {
		AvatarTable = AvatarTableFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<UNameTag> NameTagFinder(TEXT("/Game/UI/Common/WBP_NameTag.WBP_NameTag_C"));
	if (NameTagFinder.Succeeded()) {
		NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTag"));
		NameTagWidget->SetWidgetClass(NameTagFinder.Class);
		NameTagWidget->SetWidgetSpace(EWidgetSpace::World);
		NameTagWidget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		NameTagWidget->SetRelativeLocation(FVector(0.0f, 10.0f, -50.0f));
		NameTagWidget->SetCollisionProfileName(TEXT("NoCollision"));
		NameTagWidget->SetSimulatePhysics(false);
		NameTagWidget->SetBlendMode(EWidgetBlendMode::Transparent);
		NameTagWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NameTagWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	const TObjectPtr<UCapsuleComponent> Capsule = GetCapsuleComponent();
	if (Capsule) {
		Capsule->SetCapsuleSize(16.0f, 33.0f);
		Capsule->SetCollisionProfileName(TEXT("Player"));
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Capsule->SetSimulatePhysics(true);
	}

	const TObjectPtr<UCharacterMovementComponent> Movement = GetCharacterMovement();
	if (Movement) {
		Movement->GravityScale = 1.5f;
		Movement->MaxWalkSpeed = 180.0f;
		Movement->JumpZVelocity = 500.0f;
	}

	GetSprite()->SetRelativeLocation(FVector(0.0f, 1.0f, 0.0f));
	GetSprite()->SetFlipbook(IdleAnimation);

	PlayerStat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
}

void AMsPlayerBase::BeginPlay() {
	Super::BeginPlay();

	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		const FVector SpawnPointLocation = PlayerStart->GetActorLocation();
		const FVector SpawnLocation = FVector(SpawnPointLocation.X + X, SpawnPointLocation.Y, SpawnPointLocation.Z + Y + 5);
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
	X = Info.x();
	Y = Info.y();
	InitAnimation();
}

void AMsPlayerBase::Setup(const protocol::OtherPlayerInfo& Info) {
	PlayerStat->Setup(Info);
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
	X = Info.x();
	Y = Info.y();
	InitAnimation();
}

void AMsPlayerBase::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (MovementComponent->Velocity.Length() > 0) {
		AnimationType = EPlayerAnimationType::Run;
	} else {
		AnimationType = EPlayerAnimationType::Idle;
	}

	if (MovementComponent->IsFalling()) {
		AnimationType = EPlayerAnimationType::Fall;
	}

	UpdateAnimation();

	if (MovementComponent->Velocity.X > 0) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	} else if (MovementComponent->Velocity.X < 0) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}

	if (!NameTagWidget) return;
	if (NameTagWidget->GetDrawSize().X != 500) return;

	if (const auto Widget = Cast<UNameTag>(NameTagWidget->GetUserWidgetObject())) {
		const auto Size = Widget->GetDesiredSize();

		if (Size.X > 0 && Size.Y > 0) {
			NameTagWidget->SetDrawSize(Size);
		}
	}
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
		case EPlayerAnimationType::Idle:
			GetSprite()->SetFlipbook(IdleAnimation);
			break;
		case EPlayerAnimationType::Run:
			GetSprite()->SetFlipbook(RunAnimation);
			break;
		case EPlayerAnimationType::Jump: [[fallthrough]];
		case EPlayerAnimationType::Fall:
			GetSprite()->SetFlipbook(JumpAnimation);
			break;
	}
}
