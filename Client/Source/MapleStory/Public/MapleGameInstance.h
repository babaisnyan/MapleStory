#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"

#include "Data/Enum/ELoginState.h"
#include "Engine/GameInstance.h"

#include "MapleGameInstance.generated.h"

class FPacketSession;

UCLASS()
class MAPLESTORY_API UMapleGameInstance : public UGameInstance {
	GENERATED_BODY()

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

	void ChnageMap(int32 NewMapId);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELoginState LoginState = ELoginState::Login;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsConnected = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 MapId = 0;

private:
	FSocket* Socket = nullptr;
	FString LoginIpAddress = TEXT("127.0.0.1");
	int16 LoginPort = 7777;

	TSharedPtr<FPacketSession> Session;
};
