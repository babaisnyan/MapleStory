// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MsPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MapleGameInstance.h"
#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "Characters/PlayerCamera.h"
#include "Components/CapsuleComponent.h"
#include "Components/PlayerStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Data/Enum/EPlayerAnimationType.h"
#include "Data/Enum/ESoundEffectType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Managers/SoundManager.h"
#include "UI/NameTag.h"
#include "UI/StatusBarHud.h"


AMsPlayer::AMsPlayer() {
	const TObjectPtr<UCapsuleComponent> capsule = GetCapsuleComponent();

	if (capsule) {
		capsule->SetCapsuleSize(16.0f, 33.0f);

		capsule->SetCollisionProfileName(TEXT("Player"));
		capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}

	const TObjectPtr<UCharacterMovementComponent> movement = GetCharacterMovement();

	if (movement) {
		movement->GravityScale = 1.5f;
		movement->MaxWalkSpeed = 180.0f;
		movement->JumpZVelocity = 500.0f;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> DefaultContextFinder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Misc/Input/IMC_Default.IMC_Default'"));
	if (DefaultContextFinder.Succeeded()) {
		DefaultContext = DefaultContextFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Jump.IA_Jump'"));
	if (JumpActionFinder.Succeeded()) {
		JumpAction = JumpActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveHorizontalActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_MoveHorizontal.IA_MoveHorizontal'"));
	if (MoveHorizontalActionFinder.Succeeded()) {
		MoveHorizontalAction = MoveHorizontalActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveVerticalActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_MoveVertical.IA_MoveVertical'"));
	if (MoveVerticalActionFinder.Succeeded()) {
		MoveVerticalAction = MoveVerticalActionFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<UStatusBarHud> StatusBarHudFinder(TEXT("/Game/UI/HUD/WB_StatusBar.WB_StatusBar_C"));
	if (StatusBarHudFinder.Succeeded()) {
		StatusBarHudClass = StatusBarHudFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UNameTag> NameTagFinder(TEXT("/Game/UI/Common/WBP_NameTag.WBP_NameTag_C"));
	if (NameTagFinder.Succeeded()) {
		NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTag"));
		NameTagWidget->SetWidgetClass(NameTagFinder.Class);
		NameTagWidget->SetWidgetSpace(EWidgetSpace::World);
		NameTagWidget->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		NameTagWidget->SetRelativeLocation(FVector(-2.0f, 0.0f, -50.0f));
		NameTagWidget->SetBlendMode(EWidgetBlendMode::Transparent);
		NameTagWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	PlayerStat = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("PlayerStat"));
}

void AMsPlayer::BeginPlay() {
	Super::BeginPlay();

	SoundManager = GetGameInstance()->GetSubsystem<USoundManager>();

	const TObjectPtr<AActor> PlayerStart = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());

	if (PlayerStart) {
		SetActorLocation(PlayerStart->GetActorLocation());
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())) {
		auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		InputSystem->AddMappingContext(DefaultContext, 0);

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		PlayerCamera = GetWorld()->SpawnActor<APlayerCamera>(APlayerCamera::StaticClass(), GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);

		PlayerController->SetViewTargetWithBlend(PlayerCamera);
		PlayerController->bAutoManageActiveCameraTarget = false;
		PlayerController->Possess(this);
		PlayerCamera->Player = this;
	}

	if (StatusBarHudClass) {
		UStatusBarHud* Window = CreateWidget<UStatusBarHud>(GetWorld(), StatusBarHudClass);
		StatusBarHud = Window;
		Window->AddToViewport();
		UpdateStatusBar();
	}

	if (NameTagWidget) {
		if (const auto Widget = Cast<UNameTag>(NameTagWidget->GetUserWidgetObject())) {
			Widget->NameTagText->SetText(FText::FromString(Name));
		}
	}
}

void AMsPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMsPlayer::EnhancedJump);
		EnhancedInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &AMsPlayer::EnhancedMoveHorizontal);
		EnhancedInputComponent->BindAction(MoveVerticalAction, ETriggerEvent::Triggered, this, &AMsPlayer::EnhancedMoveVertical);
	}
}

void AMsPlayer::Tick(const float DeltaSeconds) {
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

	if(!NameTagWidget) return;
	if (const auto Widget = Cast<UNameTag>(NameTagWidget->GetUserWidgetObject())) {
		const auto Size = Widget->GetDesiredSize();
		
		if(Size.X > 0 && Size.Y > 0) {
			NameTagWidget->SetDrawSize(Size);
		}
	}
}

void AMsPlayer::UpdateAnimation() const {
	switch (AnimationType) {
		case EPlayerAnimationType::Idle:
			GetSprite()->SetFlipbook(IdleAnimation);
			break;
		case EPlayerAnimationType::Run:
			GetSprite()->SetFlipbook(RunAnimation);
			break;
		case EPlayerAnimationType::Jump:
			[[fallthrough]];
		case EPlayerAnimationType::Fall:
			GetSprite()->SetFlipbook(JumpAnimation);
			break;
	}
}

void AMsPlayer::UpdateStatusBar() const {
	if (!StatusBarHud) {
		return;
	}

	const int32 Temp = FMath::RandRange(1000, 10000);
	PlayerStat->Exp = FMath::RandRange(100, Temp);
	StatusBarHud->UpdateGauge(PlayerStat->Hp, PlayerStat->MaxHp, PlayerStat->Mp, PlayerStat->MaxMp, PlayerStat->Exp, Temp);

	StatusBarHud->UpdateLevel(FMath::RandRange(1, 300));
	StatusBarHud->SetName(Name);
}

void AMsPlayer::EnhancedMoveHorizontal(const FInputActionValue& Value) {
	const FVector2D AxisValue = Value.Get<FVector2D>();
	AnimationType = EPlayerAnimationType::Run;
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue.X);
}

void AMsPlayer::EnhancedMoveVertical(const FInputActionValue& Value) {
	const FVector2D AxisValue = Value.Get<FVector2D>();

	//TODO: Check if the player is colliding with the rope or ladder

	if (AxisValue.X > 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Move Up"));
	} else if (AxisValue.X < 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Move Down"));
	}
}

void AMsPlayer::EnhancedJump(const FInputActionValue& Value) {
	if (Value.Get<bool>() && !GetMovementComponent()->IsFalling()) {
		PlayerStat->Hp = FMath::RandRange(1, PlayerStat->MaxHp);
		PlayerStat->Mp = FMath::RandRange(1, PlayerStat->MaxMp);
		UpdateStatusBar();

		Jump();
		AnimationType = EPlayerAnimationType::Jump;

		SoundManager->PlaySoundEffect(ESoundEffectType::Jump, GetWorld());
	}
}

void AMsPlayer::Setup(const protocol::PlayerInfo& Info) {
	PlayerStat->Setup(Info);
	AvatarType = static_cast<EAvatarType>(Info.type());
	Name = UTF8_TO_TCHAR(Info.name().c_str());
}
