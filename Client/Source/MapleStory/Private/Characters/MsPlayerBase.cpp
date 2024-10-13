#include "Characters/MsPlayerBase.h"

#include "PaperFlipbookActor.h"
#include "PaperFlipbookComponent.h"
#include "Actors/DamageTextActor.h"
#include "Actors/SelfDestroyFlipbookActor.h"
#include "Components/CapsuleComponent.h"
#include "Components/PlayerStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Data/Table/AvatarData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ChatBalloon.h"
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

	static ConstructorHelpers::FClassFinder<UChatBalloon> ChatBalloonFinder(TEXT("/Game/UI/Common/WBP_PlayerChatBalloon.WBP_PlayerChatBalloon_C"));
	if (ChatBalloonFinder.Succeeded()) {
		ChatBalloonClass = ChatBalloonFinder.Class;
		ChatBalloonWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ChatBalloon"));
		ChatBalloonWidget->SetWidgetClass(ChatBalloonClass);
		ChatBalloonWidget->SetWidgetSpace(EWidgetSpace::Screen);
		ChatBalloonWidget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		ChatBalloonWidget->SetRelativeLocation(FVector(0.0f, 10.0f, 70.0f));
		ChatBalloonWidget->SetCollisionProfileName(TEXT("NoCollision"));
		ChatBalloonWidget->SetGenerateOverlapEvents(false);
		ChatBalloonWidget->SetSimulatePhysics(false);
		ChatBalloonWidget->CanCharacterStepUpOn = ECB_No;
		ChatBalloonWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		ChatBalloonWidget->TranslucencySortPriority = 1001;
		ChatBalloonWidget->SetVisibility(false);
		ChatBalloonWidget->SetDrawAtDesiredSize(true);
	}

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> DeadAnimationFinder(TEXT("/Game/Characters/FB_Tomb.FB_Tomb"));
	if (DeadAnimationFinder.Succeeded()) {
		DeadAnimation = DeadAnimationFinder.Object;
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
	GetSprite()->TranslucencySortPriority = 1000000;
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &AMsPlayerBase::OnAttackFinished);

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
	ObjectId = Info.object_id();
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
	StartX = Info.x();
	StartY = Info.y();
	bIsLocalPlayer = true;
	InitAnimation();
}

void AMsPlayerBase::Setup(const protocol::OtherPlayerInfo& Info) {
	PlayerStat->Setup(Info);
	ObjectId = Info.object_id();
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
	StartX = Info.x();
	StartY = Info.y();
	bIsLocalPlayer = false;
	InitAnimation();
}

void AMsPlayerBase::Move(const protocol::GameServerPlayerMove& MovePacket) {
	bFlip = MovePacket.flip();
	DestX = MovePacket.x();
	DestY = MovePacket.y();

	if (!bIsAttacking) {
		AnimationType = MovePacket.animation();
	}

	if (!bFlip) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	} else {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}

	UpdateAnimation();
}

void AMsPlayerBase::Teleport(const float X, const float Y) {
	DestX = X;
	DestY = Y;
	bFlip = false;
	SetActorLocation(FVector(DestX + BaseX, GetActorLocation().Y, DestY + BaseY));
}

void AMsPlayerBase::Blink() {
	Transparency = Transparency == 0.5f ? 1.0f : 0.5f;
	GetSprite()->SetSpriteColor(FLinearColor{1.0f, 1.0f, 1.0f, Transparency});

	if (++BlinkCount == 5) {
		GetWorldTimerManager().ClearTimer(BlinkTimer);
		GetSprite()->SetSpriteColor(FLinearColor{1.0f, 1.0f, 1.0f, 1.0f});
		Transparency = 1.0f;
		BlinkCount = 0;
		bIsInvincible = false;
	}
}

void AMsPlayerBase::HideChatBalloon() {
	if (ChatBalloonWidget) {
		ChatBalloonWidget->SetVisibility(false);
		GetWorldTimerManager().ClearTimer(ChatBalloonTimer);
	}
}

void AMsPlayerBase::OnChat(const FString& Text) {
	if (ChatBalloonWidget) {
		if (const auto Widget = Cast<UChatBalloon>(ChatBalloonWidget->GetUserWidgetObject())) {
			Widget->SetText(Text);
			ChatBalloonWidget->SetVisibility(true);

			if (GetWorldTimerManager().IsTimerActive(ChatBalloonTimer)) {
				GetWorldTimerManager().ClearTimer(ChatBalloonTimer);
			}

			GetWorldTimerManager().SetTimer(ChatBalloonTimer, this, &AMsPlayerBase::HideChatBalloon, 5.0f, false);
		}
	}
}

void AMsPlayerBase::OnAttack() {
	GetSprite()->SetLooping(false);
	GetSprite()->SetFlipbook(AttackAnimation);
	GetSprite()->Play();

	AnimationType = protocol::PLAYER_ANIMATION_ATTACK;
	bIsAttacking = true;
}

void AMsPlayerBase::OnLevelUp() {
	const TObjectPtr<UPaperFlipbook> LevelUpAnimation = LoadObject<UPaperFlipbook>(nullptr, TEXT("/Game/Characters/FB_LevelUp.FB_LevelUp"));
	const FVector Location = GetActorLocation();
	const FVector NewLocation = FVector(Location.X, Location.Y + 10, Location.Z + 15);
	ASelfDestroyFlipbookActor* LevelUp = GetWorld()->SpawnActorDeferred<ASelfDestroyFlipbookActor>(ASelfDestroyFlipbookActor::StaticClass(), FTransform::Identity, this);
	
	LevelUp->GetRenderComponent()->SetFlipbook(LevelUpAnimation);
	LevelUp->GetRenderComponent()->SetLooping(false);
	LevelUp->FinishSpawning(FTransform(NewLocation));
}

void AMsPlayerBase::OnDamaged(const int32 Damage) {
	const FVector Location = GetActorLocation();
	const FVector NewLocation = FVector(Location.X, Location.Y + 1, Location.Z + 20.0f);
	ADamageTextActor* Text = GetWorld()->SpawnActorDeferred<ADamageTextActor>(ADamageTextActor::StaticClass(), FTransform(NewLocation), this);
	Text->SetDamage(Damage, true, false);
	Text->FinishSpawning(FTransform(NewLocation));

	if (Damage > 0 && PlayerStat->Hp - Damage > 0) {
		bIsInvincible = true;
		GetWorldTimerManager().SetTimer(BlinkTimer, this, &AMsPlayerBase::Blink, 0.3f, true, 0);
	}
}

void AMsPlayerBase::OnDead() {
	bIsDead = true;
	GetSprite()->SetFlipbook(DeadAnimation);
}

void AMsPlayerBase::OnRevive() {
	bIsDead = false;
	AnimationType = protocol::PLAYER_ANIMATION_IDLE;
	GetSprite()->SetFlipbook(IdleAnimation);
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
		AttackAnimation = AvatarData->AttackAnimation;

		GetSprite()->SetFlipbook(IdleAnimation);
	}
}

void AMsPlayerBase::UpdateAnimation() const {
	if (bIsDead) {
		return;
	}

	switch (AnimationType) {
		case protocol::PLAYER_ANIMATION_RUN:
			GetSprite()->SetFlipbook(RunAnimation);
			break;
		case protocol::PLAYER_ANIMATION_JUMP:
			GetSprite()->SetFlipbook(JumpAnimation);
			break;
		case protocol::PLAYER_ANIMATION_ATTACK:
			break;
		default:
			GetSprite()->SetFlipbook(IdleAnimation);
			break;
	}
}

void AMsPlayerBase::OnAttackFinished() {
	if (bIsAttacking) {
		AnimationType = protocol::PLAYER_ANIMATION_IDLE;
		GetSprite()->SetLooping(true);
		GetSprite()->SetFlipbook(IdleAnimation);
		GetSprite()->Play();
		bIsAttacking = false;
	}
}
