#include "Characters/MsLocalPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Characters/PlayerCamera.h"
#include "Components/CapsuleComponent.h"
#include "Components/PlayerStatComponent.h"
#include "Data/Enum/ESoundEffectType.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Managers/SoundManager.h"
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

	const TObjectPtr<UCapsuleComponent> Capsule = GetCapsuleComponent();
	if (Capsule) {
		Capsule->SetSimulatePhysics(false);
	}
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
		Window->AddToViewport();
		UpdateStatusBar();
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
}

void AMsLocalPlayer::EnhancedMoveHorizontal(const FInputActionValue& Value) {
	const FVector2D AxisValue = Value.Get<FVector2D>();
	AnimationType = EPlayerAnimationType::Run;
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue.X);

	const FVector ActorLocation = GetActorLocation();
	UE_LOG(LogTemp, Display, TEXT("X: %f, Z(Y): %f"), ActorLocation.X, ActorLocation.Z);
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
		PlayerStat->Hp = FMath::RandRange(1, PlayerStat->MaxHp);
		PlayerStat->Mp = FMath::RandRange(1, PlayerStat->MaxMp);
		UpdateStatusBar();

		Jump();
		AnimationType = EPlayerAnimationType::Jump;

		SoundManager->PlaySoundEffect(ESoundEffectType::Jump, GetWorld());
	}
}

void AMsLocalPlayer::UpdateStatusBar() const {
	if (!StatusBarHud) {
		return;
	}

	const int32 Temp = FMath::RandRange(1000, 10000);
	PlayerStat->Exp = FMath::RandRange(100, Temp);
	StatusBarHud->UpdateGauge(PlayerStat->Hp, PlayerStat->MaxHp, PlayerStat->Mp, PlayerStat->MaxMp, PlayerStat->Exp, Temp);

	StatusBarHud->UpdateLevel(FMath::RandRange(1, 300));
	StatusBarHud->SetName(Name);
}
