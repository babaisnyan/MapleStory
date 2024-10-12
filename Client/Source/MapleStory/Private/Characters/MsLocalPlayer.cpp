#include "Characters/MsLocalPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MapleGameInstance.h"
#include "PaperFlipbookComponent.h"
#include "Actors/Monster.h"
#include "Characters/PlayerCamera.h"
#include "Components/MobStatComponent.h"
#include "Components/PlayerStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameModes/MapleGameMode.h"
#include "Managers/KeySettingManager.h"
#include "Managers/SoundManager.h"
#include "Network/PacketCreator.h"
#include "UI/ChatWidget.h"
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

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Attack.IA_Attack'"));
	if (AttackActionFinder.Succeeded()) {
		AttackAction = AttackActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EnterActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Enter.IA_Enter'"));
	if (EnterActionFinder.Succeeded()) {
		EnterAction = EnterActionFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveHorizontalActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_MoveHorizontal.IA_MoveHorizontal'"));
	if (MoveHorizontalActionFinder.Succeeded()) {
		MoveHorizontalAction = MoveHorizontalActionFinder.Object;
	}

	static ConstructorHelpers::FClassFinder<UStatusBarHud> StatusBarHudFinder(TEXT("/Game/UI/HUD/WB_StatusBar.WB_StatusBar_C"));
	if (StatusBarHudFinder.Succeeded()) {
		StatusBarHudClass = StatusBarHudFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UQuickSlotWidget> QuickSlotWidgetFinder(TEXT("/Game/UI/HUD/WBP_QuickSlot.WBP_QuickSlot_C"));
	if (QuickSlotWidgetFinder.Succeeded()) {
		QuickSlotWidgetClass = QuickSlotWidgetFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UChatWidget> ChatWidgetFinder(TEXT("/Game/UI/HUD/WBP_Chat.WBP_Chat_C"));
	if (ChatWidgetFinder.Succeeded()) {
		ChatWidgetClass = ChatWidgetFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> DeathNoticeFinder(TEXT("/Game/UI/Common/WBP_DeadNotice.WBP_DeadNotice_C"));
	if (DeathNoticeFinder.Succeeded()) {
		DeathNoticeClass = DeathNoticeFinder.Class;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bEnablePhysicsInteraction = false;
	GetSprite()->TranslucencySortPriority = 1000001;
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &AMsLocalPlayer::OnAttackFinished);
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
		StatusBarHud = CreateWidget<UStatusBarHud>(GetWorld(), StatusBarHudClass);
		StatusBarHud->AddToViewport(1);
		UpdateStatusBar();
	}

	if (QuickSlotWidgetClass) {
		QuickSlotWidget = CreateWidget<UQuickSlotWidget>(GetWorld(), QuickSlotWidgetClass);
		QuickSlotWidget->AddToViewport(2);
	}

	if (ChatWidgetClass) {
		ChatWidget = CreateWidget<UChatWidget>(GetWorld(), ChatWidgetClass);
		ChatWidget->AddToViewport(3);
	}

	AddMovementInput(FVector(10.0f, 0.0f, 0.0f), 1.0f);
}

void AMsLocalPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedJump);
		EnhancedInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedMoveHorizontal);
		EnhancedInputComponent->BindAction(EnterAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::Enter);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMsLocalPlayer::Attack);
	}
}

void AMsLocalPlayer::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (!bIsAttacking) {
		if (MovementComponent->Velocity.Length() > 0) {
			AnimationType = protocol::PLAYER_ANIMATION_RUN;
		} else {
			AnimationType = protocol::PLAYER_ANIMATION_IDLE;
		}

		if (MovementComponent->IsFalling()) {
			AnimationType = protocol::PLAYER_ANIMATION_JUMP;
		}
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

		bFirstSent = true;
		LastMovePacketLocation = NewLocation;
		LastAnimationType = AnimationType;
		MovePacketSendTimer = 0.1f;
		UpdatePosition();
		const auto SendBuffer = FPacketCreator::GetClientMove(NewLocation.X, NewLocation.Z, bFlip, AnimationType);
		SEND_PACKET(SendBuffer);
	}

	if (!bIsAttacking) {
		UpdateAnimation();
	}
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

		if (PlayerStat->Hp > 0) {
			GetCharacterMovement()->AddImpulse({bFlip ? 200.0f : -200.0f, 0.0f, 200.0f}, true);
		}
	}
}

void AMsLocalPlayer::OnDead() {
	Super::OnDead();

	if (!DeathNoticeClass) {
		return;
	}

	DeathNotice = CreateWidget<UUserWidget>(GetGameInstance(), DeathNoticeClass);
	DeathNotice->AddToViewport(100);
}

void AMsLocalPlayer::OnRevive() {
	Super::OnRevive();

	bSentRevive = false;

	if (DeathNotice) {
		DeathNotice->RemoveFromParent();
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		const FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
}

void AMsLocalPlayer::OnClickedRevive() {
	if (bSentRevive) {
		return;
	}

	const auto Packet = FPacketCreator::GetReviveRequest();
	SEND_PACKET(Packet);
	bSentRevive = true;
}

void AMsLocalPlayer::EnhancedMoveHorizontal(const FInputActionValue& Value) {
	if (bIsDead || bIsAttacking) {
		return;
	}

	const FVector2D AxisValue = Value.Get<FVector2D>();
	AnimationType = protocol::PLAYER_ANIMATION_RUN;
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue.X);
}

void AMsLocalPlayer::EnhancedJump(const FInputActionValue& Value) {
	if (bIsDead || bIsAttacking) {
		return;
	}

	if (Value.Get<bool>() && !GetMovementComponent()->IsFalling()) {
		Jump();
		AnimationType = protocol::PLAYER_ANIMATION_JUMP;
		SoundManager->PlaySoundEffect(ESoundEffectType::Jump, GetWorld());
	}
}

void AMsLocalPlayer::Enter(const FInputActionValue& Value) {
	if (!ChatWidget) {
		return;
	}

	if (Value.Get<bool>()) {
		ChatWidget->ToggleChat();
	}
}

void AMsLocalPlayer::Attack(const FInputActionValue& Value) {
	if (bIsDead) {
		return;
	}

	const float Now = GetWorld()->GetTimeSeconds();

	if (FMath::Abs(Now - LastAttackTime) < 0.7f) {
		return;
	}

	if (Value.Get<bool>()) {
		UE_LOG(LogTemp, Warning, TEXT("Attack"));

		GetSprite()->SetLooping(false);
		GetSprite()->SetFlipbook(AttackAnimation);
		GetSprite()->Play();

		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMsLocalPlayer::CheckHitMob, 0.5f, false);

		bIsAttacking = true;
		LastAttackTime = Now;
	}
}

void AMsLocalPlayer::OnAttackFinished() {
	if (bIsAttacking) {
		AnimationType = protocol::PLAYER_ANIMATION_IDLE;
		GetSprite()->SetLooping(true);
		GetSprite()->SetFlipbook(IdleAnimation);
		GetSprite()->Play();
		bIsAttacking = false;
	}
}

void AMsLocalPlayer::CheckHitMob() {
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * (bFlip ? -1.0f : 1.0f) * 100.0f;
	Start.Y = 1.0f;
	End.Y = 1.0f;

	FHitResult HitResult;
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel1);

	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f);

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, QueryParams)) {
		const AMonster* Mob = Cast<AMonster>(HitResult.GetActor());
		if (Mob && Mob->StatComponent->Hp > 0) {
			const auto Packet = FPacketCreator::GetAttackRequest(Mob->ObjectId);
			SEND_PACKET(Packet);

			return;
		}
	}

	const auto Packet = FPacketCreator::GetAttackRequest(0);
	SEND_PACKET(Packet);
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
