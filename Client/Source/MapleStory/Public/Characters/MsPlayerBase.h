#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "game_protocol.pb.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"
#include "MsPlayerBase.generated.h"

class UChatBalloon;
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

	void Teleport(float X, float Y);
	void Blink();
	void HideChatBalloon();
	void OnChat(const FString& Text);
	virtual void OnDamaged(int32 Damage);
	virtual void OnDead();
	virtual void OnRevive();

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UPaperFlipbook> DeadAnimation;
	
	protocol::PlayerAnimation AnimationType = protocol::PlayerAnimation::PLAYER_ANIMATION_IDLE;

	UPROPERTY()
	uint64 ObjectId;
	
	UPROPERTY(EditDefaultsOnly, Category = "Avatar")
	EAvatarType AvatarType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	FString Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<UPlayerStatComponent> PlayerStat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool bIsLocalPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool bIsInvincible = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> NameTagWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UWidgetComponent> ChatBalloonWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UChatBalloon> ChatBalloonClass;

	UPROPERTY()
	TObjectPtr<UDataTable> AvatarTable;
	
	bool bFlip = true;
	int32 StartX;
	int32 StartY;
	float BaseX;
	float BaseY;
	float DestX;
	float DestY;

	UPROPERTY()
	FTimerHandle BlinkTimer;
	float Transparency = 1.0f;
	int32 BlinkCount = 0;

	UPROPERTY()
	FTimerHandle ChatBalloonTimer;

	bool bIsDead = false;
};
