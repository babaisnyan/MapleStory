#include "Characters/LoginCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameModes/LoginGameMode.h"

void ALoginCharacter::ChangeSelection(const FInputActionValue& Value) {
	const float AxisValue = Value.Get<float>();

	if (AxisValue > 0) {
		SelectedAvatarIndex = FMath::Min(SelectedAvatarIndex + 1, MaxAvatarCount - 1);
	} else if (AxisValue < 0) {
		SelectedAvatarIndex = FMath::Max(SelectedAvatarIndex - 1, 0);
	}

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		GameMode->SelectedPlayerIndex = SelectedAvatarIndex;
		GameMode->bCharacterSelected = true;
	}
}

void ALoginCharacter::OnGameStartButton(const FInputActionValue& Value) {
	const bool Pressed = Value.Get<bool>();

	if (Pressed) {
		ALoginGameMode* GameMode = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());

		if (GameMode) {
			GameMode->StartGame();
		}
	}
}

void ALoginCharacter::SetAvatar(const int32 Index, const EAvatarType AvatarType, const FString& AvatarName) {
	check(Index >= 0 && Index < Avatars.Num());

	Avatars[Index] = AvatarType;
	AvatarNames[Index] = AvatarName;
	SelectedAvatarIndex = 0;
	MaxAvatarCount++;

	ALoginGameMode* GameMode = Cast<ALoginGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode) {
		GameMode->SelectedPlayerIndex = SelectedAvatarIndex;
		GameMode->bCharacterSelected = true;
	}
}

void ALoginCharacter::DeleteAvatar(const int32 Index) {
	check(Index >= 0 && Index < Avatars.Num());

	Avatars[Index] = EAvatarType::None;
	AvatarNames[Index] = "";

	TArray<EAvatarType> NewAvatars;
	TArray<FString> NewAvatarNames;
	NewAvatars.Reserve(6);
	NewAvatarNames.Reserve(6);

	int32 AddCount = 0;

	for (int i = 0; i < Avatars.Num(); i++) {
		if (Avatars[i] != EAvatarType::None) {
			NewAvatars.Add(Avatars[i]);
			NewAvatarNames.Add(AvatarNames[i]);
			AddCount++;
		}
	}

	for (int i = 0; i < 6 - AddCount; i++) {
		NewAvatars.Add(EAvatarType::None);
		NewAvatarNames.Add("");
	}

	Avatars = NewAvatars;
	AvatarNames = NewAvatarNames;
	MaxAvatarCount = FMath::Max(MaxAvatarCount - 1, 0);

	if (Avatars[0] == EAvatarType::None) {
		SelectedAvatarIndex = -1;
	} else {
		SelectedAvatarIndex = 0;
	}
}

void ALoginCharacter::BeginPlay() {
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController())) {
		auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		InputSystem->AddMappingContext(InputContext, 0);
	}
}

void ALoginCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(CharSelectionAction, ETriggerEvent::Triggered, this, &ALoginCharacter::ChangeSelection);
		EnhancedInputComponent->BindAction(GameStartAction, ETriggerEvent::Triggered, this, &ALoginCharacter::OnGameStartButton);
	}
}
