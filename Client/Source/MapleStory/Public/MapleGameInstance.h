#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"
#include "game_struct.pb.h"
#include "Data/Enum/EAvatarType.h"

#include "Data/Enum/ELoginState.h"
#include "Engine/GameInstance.h"

#include "MapleGameInstance.generated.h"

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
	}
	
public:
	UFUNCTION(BlueprintCallable)
	bool ConnectToLoginServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromServer();

	bool ConnectToGameServer(const FString& IpAddress, int16 Port);

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	void SendPacket(const FSendBufferRef& SendBuffer) const;

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets() const;

	virtual void Shutdown() override;

	void QuitGame();

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLoginState(ELoginState NewState);
	
	void ChangeMap(int32 NewMapId);

	void AddPlayer(const protocol::OtherPlayerInfo& OtherPlayerInfo);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELoginState LoginState = ELoginState::Login;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsConnected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MapId = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AMsPlayer> CurrentPlayer;
	
	TOptional<protocol::PlayerInfo> PlayerInfoTemp = TOptional<protocol::PlayerInfo>();
	
private:
	FSocket* Socket = nullptr;
	FString LoginIpAddress = TEXT("127.0.0.1");
	int16 LoginPort = 7777;

	TSharedPtr<FPacketSession> Session;
};
