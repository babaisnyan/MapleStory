#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"
#include "game_protocol.pb.h"
#include "game_struct.pb.h"

#include "Data/Enum/ELoginState.h"
#include "Engine/GameInstance.h"

#include "MapleGameInstance.generated.h"

class UTCPClientComponent;
class AMsLocalPlayer;
class AMsPlayer;
class FPacketSession;

UCLASS()
class MAPLESTORY_API UMapleGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	virtual ~UMapleGameInstance() override {
		if (PlayerInfoTemp) {
			PlayerInfoTemp.Reset();
			PlayerInfoTemp = TOptional<protocol::PlayerInfo>();
		}

		OtherPlayersQueue.Empty();
	}

	virtual void Init() override;
	virtual void BeginDestroy() override;

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToLoginServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromServer();

	void ConnectToGameServer(const FString& IpAddress, int16 Port);

	void EnqueueSendPacket(const FSendBufferRef& SendBuffer);
	void ClearSendQueue();
	void SendPacket(const FSendBufferRef& SendBuffer) const;

	virtual void Shutdown() override;

	void QuitGame() const;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLoginState(ELoginState NewState);
	void ChangeMap(int32 NewMapId);
	void AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo);
	void AddMonster(const protocol::MobInfo& MonsterInfo);
	void RemoveObject(int64 ObjectId);
	void UpdatePlayerPosition(const protocol::GameServerPlayerMove& Packet) const;
	void UpdateMonsterPosition(const protocol::GameServerMobMove& Packet) const;
	void SetMobAgro(const protocol::GameServerMobAgro& Packet) const;
	void RemoveMobAgro(const protocol::GameServerRemoveMobAgro& Packet) const;
	void PlayAttackAnimation(const protocol::GameServerMobAttack& Packet) const;

private:
	UFUNCTION()
	void OnLoginServerConnected();

	UFUNCTION()
	void OnReceivedBytesLogin(const TArray<uint8>& Bytes);

	UFUNCTION()
	void OnGameServerConnected();

	UFUNCTION()
	void OnReceivedBytesGame(const TArray<uint8>& Bytes);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELoginState LoginState = ELoginState::Login;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsConnected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MapId = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AMsLocalPlayer> CurrentPlayer;

	TOptional<protocol::PlayerInfo> PlayerInfoTemp = TOptional<protocol::PlayerInfo>();

	TQueue<TOptional<protocol::OtherPlayerInfo>> OtherPlayersQueue;
	TQueue<TOptional<protocol::MobInfo>> MonstersQueue;
	TQueue<int32> RemovePlayerQueue;

private:
	UPROPERTY()
	TObjectPtr<UTCPClientComponent> Client;

	TQueue<FSendBufferRef> SendQueue;

	FString LoginIpAddress = TEXT("127.0.0.1");
	int16 LoginPort = 7777;
};
