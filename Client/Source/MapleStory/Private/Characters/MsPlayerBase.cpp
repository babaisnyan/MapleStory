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
		NameTagWidget->SetBlendMode(EWidgetBlendMode::Transparent);
		NameTagWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	const TObjectPtr<UCapsuleComponent> Capsule = GetCapsuleComponent();
	if (Capsule) {
		Capsule->SetCapsuleSize(16.0f, 33.0f);

		Capsule->SetCollisionProfileName(TEXT("Player"));
		Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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

	// TODO: 서버에서 보내준 플레이어 스폰 위치로 이동
	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		SetActorLocation(PlayerStart->GetActorLocation());
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
	InitAnimation();
}

void AMsPlayerBase::Setup(const protocol::OtherPlayerInfo& Info) {
	// PlayerStat->Setup(Info);
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
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
