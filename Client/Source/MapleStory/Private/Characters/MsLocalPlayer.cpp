#include "Characters/MsLocalPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MapleGameInstance.h"
#include "PaperFlipbookComponent.h"
#include "Actors/Monster.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Characters/PlayerCamera.h"
#include "Components/MobStatComponent.h"
#include "Components/PlayerStatComponent.h"
#include "Data/Item.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameModes/MapleGameMode.h"
#include "Managers/InventoryManager.h"
#include "Managers/ItemManager.h"
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

	for (int32 i = 1; i <= 6; i++) {
		ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotActionFinder(*FString::Printf(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_%d.IA_%d'"), i, i));
		if (QuickSlotActionFinder.Succeeded()) {
			Actions.Add(static_cast<EAction>(i - 1), QuickSlotActionFinder.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_A.IA_A'"));
	if (AActionFinder.Succeeded()) {
		Actions.Add(EAction::A, AActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> DeleteActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Delete.IA_Delete'"));
	if (DeleteActionFinder.Succeeded()) {
		Actions.Add(EAction::Delete, DeleteActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EndActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_End.IA_End'"));
	if (EndActionFinder.Succeeded()) {
		Actions.Add(EAction::End, EndActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> HomeActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Home.IA_Home'"));
	if (HomeActionFinder.Succeeded()) {
		Actions.Add(EAction::Home, HomeActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InsertActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Insert.IA_Insert'"));
	if (InsertActionFinder.Succeeded()) {
		Actions.Add(EAction::Insert, InsertActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PageDownActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Pdn.IA_Pdn'"));
	if (PageDownActionFinder.Succeeded()) {
		Actions.Add(EAction::PageDown, PageDownActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> PageUpActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Pup.IA_Pup'"));
	if (PageUpActionFinder.Succeeded()) {
		Actions.Add(EAction::PageUp, PageUpActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> QActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Q.IA_Q'"));
	if (QActionFinder.Succeeded()) {
		Actions.Add(EAction::Q, QActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_R.IA_R'"));
	if (RActionFinder.Succeeded()) {
		Actions.Add(EAction::R, RActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> ShiftActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Shift.IA_Shift'"));
	if (ShiftActionFinder.Succeeded()) {
		Actions.Add(EAction::Shift, ShiftActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_T.IA_T'"));
	if (TActionFinder.Succeeded()) {
		Actions.Add(EAction::T, TActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> WActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_W.IA_W'"));
	if (WActionFinder.Succeeded()) {
		Actions.Add(EAction::W, WActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Jump.IA_Jump'"));
	if (JumpActionFinder.Succeeded()) {
		Actions.Add(EAction::Jump, JumpActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Attack.IA_Attack'"));
	if (AttackActionFinder.Succeeded()) {
		Actions.Add(EAction::Attack, AttackActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EnterActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Enter.IA_Enter'"));
	if (EnterActionFinder.Succeeded()) {
		Actions.Add(EAction::Enter, EnterActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> EquipActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Equip.IA_Equip'"));
	if (EquipActionFinder.Succeeded()) {
		Actions.Add(EAction::Equip, EquipActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InventoryActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Inventory.IA_Inventory'"));
	if (InventoryActionFinder.Succeeded()) {
		Actions.Add(EAction::Inventory, InventoryActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> StatActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_Stat.IA_Stat'"));
	if (StatActionFinder.Succeeded()) {
		Actions.Add(EAction::Stat, StatActionFinder.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveHorizontalActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/Misc/Input/IA_MoveHorizontal.IA_MoveHorizontal'"));
	if (MoveHorizontalActionFinder.Succeeded()) {
		Actions.Add(EAction::MoveHorizontal, MoveHorizontalActionFinder.Object);
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

	static ConstructorHelpers::FClassFinder<UEquipWindow> EquipWindowFinder(TEXT("/Game/UI/Windows/WBP_EquipWindow.WBP_EquipWindow_C"));
	if (EquipWindowFinder.Succeeded()) {
		EquipWindowClass = EquipWindowFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UInventoryWindow> InventoryWindowFinder(TEXT("/Game/UI/Windows/WBP_InventoryWindow.WBP_InventoryWindow_C"));
	if (InventoryWindowFinder.Succeeded()) {
		InventoryWindowClass = InventoryWindowFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UStatWindow> StatWindowFinder(TEXT("/Game/UI/Windows/WBP_StatWindow.WBP_StatWindow_C"));
	if (StatWindowFinder.Succeeded()) {
		StatWindowClass = StatWindowFinder.Class;
	}

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bEnablePhysicsInteraction = false;
	GetSprite()->TranslucencySortPriority = 1000001;
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
		EnhancedInputComponent->BindAction(Actions[EAction::Jump], ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedJump);
		EnhancedInputComponent->BindAction(Actions[EAction::MoveHorizontal], ETriggerEvent::Triggered, this, &AMsLocalPlayer::EnhancedMoveHorizontal);
		EnhancedInputComponent->BindAction(Actions[EAction::Enter], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Enter);
		EnhancedInputComponent->BindAction(Actions[EAction::Attack], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Attack);
		EnhancedInputComponent->BindAction(Actions[EAction::Equip], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Equip);
		EnhancedInputComponent->BindAction(Actions[EAction::Inventory], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Inventory);
		EnhancedInputComponent->BindAction(Actions[EAction::Stat], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Stat);
		EnhancedInputComponent->BindAction(Actions[EAction::Num1], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Num1);
		EnhancedInputComponent->BindAction(Actions[EAction::Num2], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Num2);
		EnhancedInputComponent->BindAction(Actions[EAction::Num3], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Num3);
		EnhancedInputComponent->BindAction(Actions[EAction::Num4], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Num4);
		EnhancedInputComponent->BindAction(Actions[EAction::Num5], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Num5);
		EnhancedInputComponent->BindAction(Actions[EAction::Num6], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Num6);
		EnhancedInputComponent->BindAction(Actions[EAction::A], ETriggerEvent::Triggered, this, &AMsLocalPlayer::A);
		EnhancedInputComponent->BindAction(Actions[EAction::Delete], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Delete);
		EnhancedInputComponent->BindAction(Actions[EAction::End], ETriggerEvent::Triggered, this, &AMsLocalPlayer::End);
		EnhancedInputComponent->BindAction(Actions[EAction::Home], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Home);
		EnhancedInputComponent->BindAction(Actions[EAction::Insert], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Insert);
		EnhancedInputComponent->BindAction(Actions[EAction::PageDown], ETriggerEvent::Triggered, this, &AMsLocalPlayer::PageDown);
		EnhancedInputComponent->BindAction(Actions[EAction::PageUp], ETriggerEvent::Triggered, this, &AMsLocalPlayer::PageUp);
		EnhancedInputComponent->BindAction(Actions[EAction::Q], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Q);
		EnhancedInputComponent->BindAction(Actions[EAction::R], ETriggerEvent::Triggered, this, &AMsLocalPlayer::R);
		EnhancedInputComponent->BindAction(Actions[EAction::Shift], ETriggerEvent::Triggered, this, &AMsLocalPlayer::Shift);
		EnhancedInputComponent->BindAction(Actions[EAction::T], ETriggerEvent::Triggered, this, &AMsLocalPlayer::T);
		EnhancedInputComponent->BindAction(Actions[EAction::W], ETriggerEvent::Triggered, this, &AMsLocalPlayer::W);
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
	InventoryManager = GetGameInstance()->GetSubsystem<UInventoryManager>();
	ItemManager = GetGameInstance()->GetSubsystem<UItemManager>();
	KeySettingManager->Clear();
	InventoryManager->Clear();

	Meso = Info.meso();

	for (const protocol::KeySetting& Key_Setting : Info.key_settings()) {
		const EKeyCode KeyCode = static_cast<EKeyCode>(Key_Setting.key_code());
		KeySettingManager->Set(KeyCode, Key_Setting);
	}

	for (const protocol::ItemInfo& ItemInfo : Info.items()) {
		TObjectPtr<UItem> Item = NewObject<UItem>();
		Item->Template = *ItemManager->GetItemTemplate(ItemInfo.id());
		Item->Quantity = ItemInfo.count();

		switch (ItemInfo.type()) {
			case 0:
				InventoryManager->EquipInventory.Emplace(ItemInfo.pos(), Item);
				break;
			case 1:
				InventoryManager->UseInventory.Emplace(ItemInfo.pos(), Item);
				break;
			case 2:
				InventoryManager->EtcInventory.Emplace(ItemInfo.pos(), Item);
				break;
			case 3:
				InventoryManager->EquippedInventory.Emplace(ItemInfo.pos(), Item);
				break;
		}
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
		GetSprite()->SetLooping(false);
		GetSprite()->SetFlipbook(AttackAnimation);
		GetSprite()->Play();

		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AMsLocalPlayer::CheckHitMob, 0.5f, false);

		bIsAttacking = true;
		LastAttackTime = Now;
	}
}

void AMsLocalPlayer::Equip(const FInputActionValue& Value) {
	if (bIsDead) {
		return;
	}

	UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());

	if (!GameInstance) {
		return;
	}

	if (GameInstance->bIsEquipOpen) {
		return;
	}

	if (EquipWindowClass) {
		UEquipWindow* EquipWindow = CreateWidget<UEquipWindow>(GetWorld(), EquipWindowClass);
		EquipWindow->AddToViewport(4);
		EquipWindow->LoadIcons();
		GameInstance->bIsEquipOpen = true;
	}
}

void AMsLocalPlayer::Inventory(const FInputActionValue& Value) {
	if (bIsDead) {
		return;
	}

	UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());

	if (!GameInstance) {
		return;
	}

	if (GameInstance->bIsInventoryOpen) {
		return;
	}

	if (InventoryWindowClass) {
		UInventoryWindow* InventoryWindow = CreateWidget<UInventoryWindow>(GetWorld(), InventoryWindowClass);
		InventoryWindow->AddToViewport(5);
		InventoryWindow->RefreshEquip();
		GameInstance->bIsInventoryOpen = true;
	}
}

void AMsLocalPlayer::Stat(const FInputActionValue& Value) {
	if (bIsDead) {
		return;
	}

	UMapleGameInstance* GameInstance = Cast<UMapleGameInstance>(GetGameInstance());

	if (!GameInstance) {
		return;
	}

	if (GameInstance->bIsStatOpen) {
		return;
	}

	if (StatWindowClass) {
		UStatWindow* StatWindow = CreateWidget<UStatWindow>(GetWorld(), StatWindowClass);
		StatWindow->AddToViewport(6);
		GameInstance->bIsStatOpen = true;
	}
}

void AMsLocalPlayer::Num1(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::One);
	}
}

void AMsLocalPlayer::Num2(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Two);
	}
}

void AMsLocalPlayer::Num3(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Three);
	}
}

void AMsLocalPlayer::Num4(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Four);
	}
}

void AMsLocalPlayer::Num5(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Five);
	}
}

void AMsLocalPlayer::Num6(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Six);
	}
}

void AMsLocalPlayer::A(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::A);
	}
}

void AMsLocalPlayer::Delete(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Delete);
	}
}

void AMsLocalPlayer::End(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::End);
	}
}

void AMsLocalPlayer::Home(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Home);
	}
}

void AMsLocalPlayer::Insert(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Insert);
	}
}

void AMsLocalPlayer::PageDown(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::PageDown);
	}
}

void AMsLocalPlayer::PageUp(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::PageUp);
	}
}

void AMsLocalPlayer::Q(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Q);
	}
}

void AMsLocalPlayer::R(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::R);
	}
}

void AMsLocalPlayer::Shift(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::Shift);
	}
}

void AMsLocalPlayer::T(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::T);
	}
}

void AMsLocalPlayer::W(const FInputActionValue& Value) {
	if (Value.Get<bool>()) {
		OnQuickSlotKey(EKeyCode::W);
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

void AMsLocalPlayer::OnQuickSlotKey(const EKeyCode Action) {
	if (bIsDead || !QuickSlotWidget) {
		return;
	}

	if (!LastKeyTime.Contains(Action)) {
		LastKeyTime.Add(Action, GetWorld()->GetTimeSeconds());
	} else if (FMath::Abs(GetWorld()->GetTimeSeconds() - LastKeyTime[Action]) < 0.25f) {
		return;
	}

	if (!InventoryManager || !KeySettingManager) {
		return;
	}

	const TSharedPtr<protocol::KeySetting> KeySetting = KeySettingManager->Get(Action);

	if (!KeySetting) {
		return;
	}

	if (KeySetting->key_type() != protocol::KEY_TYPE_ITEM || !KeySetting->has_item_id() || KeySetting->item_id() == 0) {
		return;
	}

	const int32 Pos = InventoryManager->UseItemById(KeySetting->item_id());

	if (Pos == -1) {
		return;
	}

	TArray<UUserWidget*> Windows;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), Windows, UInventoryWindow::StaticClass(), false);

	for (const auto Object : Windows) {
		UInventoryWindow* InventoryWindow = Cast<UInventoryWindow>(Object);

		if (InventoryWindow && InventoryWindow->IsInViewport()) {
			InventoryWindow->RefreshUse();
		}
	}

	QuickSlotWidget->Refresh();
	LastKeyTime[Action] = GetWorld()->GetTimeSeconds();
	SEND_PACKET(FPacketCreator::GetUseItemRequest(Pos));
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
