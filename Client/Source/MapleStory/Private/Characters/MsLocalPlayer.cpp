#include "Characters/MsLocalPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MapleGameInstance.h"
#include "PaperFlipbookComponent.h"
#include "Characters/PlayerCamera.h"
#include "Components/PlayerStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameModes/MapleGameMode.h"
#include "Managers/KeySettingManager.h"
#include "Managers/SoundManager.h"
#include "Network/PacketCreator.h"
#include "UI/QuickSlotWidget.h"
#include "UI/StatusBarHud.h"

AMsLocalPlayer::AMsLocalPlayer() {
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

	static ConstructorHelpers::FClassFinder<UQuickSlotWidget> QuickSlotWidgetFinder(TEXT("/Game/UI/HUD/WBP_QuickSlot.WBP_QuickSlot_C"));
	if (QuickSlotWidgetFinder.Succeeded()) {
		QuickSlotWidgetClass = QuickSlotWidgetFinder.Class;
	}
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bEnablePhysicsInteraction = false;
	GetSprite()->TranslucencySortPriority = 1000001;
	
	// const TObjectPtr<UCapsuleComponent> Capsule = GetCapsuleComponent();
	// if (Capsule) {
	// 	Capsule->SetEnableGravity(true);
	// 	Capsule->SetSimulatePhysics(false);
	// }
}

void AMsLocalPlayer::BeginPlay() {
	Super::BeginPlay();

	SoundManager = GetGameInstance()->GetSubsystem<USoundManager>();

	AddActorWorldOffset(FVector(0.0f, 1.0f, 0.0f));

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
		Window->AddToViewport(1);
		UpdateStatusBar();
	}

	if (QuickSlotWidgetClass) {
		UQuickSlotWidget* Window = CreateWidget<UQuickSlotWidget>(GetWorld(), QuickSlotWidgetClass);
		QuickSlotWidget = Window;
		Window->AddToViewport(2);
	}
}

void AMsLocalPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedJump);
		EnhancedInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedMoveHorizontal);
		EnhancedInputComponent->BindAction(MoveVerticalAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedMoveVertical);
	}
}

void AMsLocalPlayer::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
	
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	
	if (MovementComponent->Velocity.Length() > 0) {
		AnimationType = protocol::PLAYER_ANIMATION_RUN;
	} else {
		AnimationType = protocol::PLAYER_ANIMATION_IDLE;
	}

	if (MovementComponent->IsFalling()) {
		AnimationType = protocol::PLAYER_ANIMATION_JUMP;
	}

	if (MovementComponent->Velocity.X > 0) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	} else if (MovementComponent->Velocity.X < 0) {
		GetSprite()->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	}

	MovePacketSendTimer -= DeltaSeconds;

	if (MovePacketSendTimer <= 0.0f) {
		const FVector Location = GetActorLocation();
		const FVector NewLocation = {Location.X - BaseX, Location.Y, Location.Z - BaseY};

		if (LastMovePacketLocation == NewLocation && LastAnimationType == protocol::PLAYER_ANIMATION_IDLE) {
			return;
		}

		LastMovePacketLocation = NewLocation;
		LastAnimationType = AnimationType;
		MovePacketSendTimer = 0.1f;
		UpdatePosition();
		const auto SendBuffer = FPacketCreator::GetClientMove(NewLocation.X, NewLocation.Z, bFlip, AnimationType);
		SEND_PACKET(SendBuffer);
	}

	UpdateAnimation();
}

void AMsLocalPlayer::Setup(const protocol::PlayerInfo& Info) {
	Super::Setup(Info);

	KeySettingManager = GetGameInstance()->GetSubsystem<UKeySettingManager>();
	KeySettingManager->Clear();

	for (const protocol::KeySetting& Key_Setting : Info.key_settings()) {
		const EKeyCode KeyCode = static_cast<EKeyCode>(Key_Setting.key_code());
		KeySettingManager->Set(KeyCode, Key_Setting);
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->MaxWalkSpeed *= PlayerStat->Speed / 100;
}

void AMsLocalPlayer::OnDamaged(const int32 Damage) {
	Super::OnDamaged(Damage);

	if (Damage > 0) {
		PlayerStat->Hp = FMath::Max(0, PlayerStat->Hp - Damage);
		UpdateStatusBar();

		GetCharacterMovement()->AddImpulse({bFlip ? 200.0f : -200.0f, 0.0f, 200.0f}, true);
	}
}

void AMsLocalPlayer::EnhancedMoveHorizontal(const FInputActionValue& Value) {
	const FVector2D AxisValue = Value.Get<FVector2D>();
	AnimationType = protocol::PLAYER_ANIMATION_RUN;
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue.X);
}

void AMsLocalPlayer::EnhancedMoveVertical(const FInputActionValue& Value) {
	const FVector2D AxisValue = Value.Get<FVector2D>();

	//TODO: Check if the player is colliding with the rope or ladder

	if (AxisValue.X > 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Move Up"));
	} else if (AxisValue.X < 0.0f) {
		UE_LOG(LogTemp, Warning, TEXT("Move Down"));
	}
}

void AMsLocalPlayer::EnhancedJump(const FInputActionValue& Value) {
	if (Value.Get<bool>() && !GetMovementComponent()->IsFalling()) {
		Jump();
		AnimationType = protocol::PLAYER_ANIMATION_JUMP;
		SoundManager->PlaySoundEffect(ESoundEffectType::Jump, GetWorld());
	}
}

void AMsLocalPlayer::UpdateStatusBar() const {
	if (!StatusBarHud) {
		return;
	}

	auto* GameMode = GetWorld()->GetAuthGameMode<AMapleGameMode>();
	uint64_t Exp = 1;
	if (GameMode) {
		Exp = GameMode->GetExpForLevel(PlayerStat->Level);
	}

	StatusBarHud->UpdateGauge(PlayerStat->Hp, PlayerStat->MaxHp, PlayerStat->Mp, PlayerStat->MaxMp, PlayerStat->Exp, Exp);
	StatusBarHud->UpdateLevel(PlayerStat->Level);
	StatusBarHud->SetName(Name);
}
