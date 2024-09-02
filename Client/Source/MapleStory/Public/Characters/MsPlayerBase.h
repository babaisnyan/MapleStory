#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "game_protocol.pb.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"
#include "MsPlayerBase.generated.h"


class UWidgetComponent;
class UPlayerStatComponent;
class UPaperFlipbook;

UCLASS()
class MAPLESTORY_API AMsPlayerBase : public APaperCharacter {
	GENERATED_BODY()

public:
	AMsPlayerBase();

public:
	virtual void BeginPlay() override;

	/*로컬 플레이어용 */
	virtual void Setup(const protocol::PlayerInfo& Info);

	/*다른 플레이어용 */
	void Setup(const protocol::OtherPlayerInfo& Info);
	void Move(const protocol::GameServerPlayerMove& MovePacket);

protected:
	virtual void Tick(float DeltaSeconds) override;
	void UpdatePosition();
	void UpdateAnimation() const;
	
private:
	void InitAnimation();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> IdleAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> RunAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> JumpAnimation;
	
	protocol::PlayerAnimation AnimationType = protocol::PlayerAnimation::PLAYER_ANIMATION_IDLE;

	UPROPERTY(EditDefaultsOnly, Category = "Avatar")
	EAvatarType AvatarType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UPlayerStatComponent> PlayerStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool bIsLocalPlayer;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> NameTagWidget;
	
	bool bFlip = true;
	int32 StartX;
	int32 StartY;
	float BaseX;
	float BaseY;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> AvatarTable;

	float DestX;
	float DestY;
};
