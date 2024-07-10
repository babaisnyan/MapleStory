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
	void DisconnectFromLoginServer();

	void SendPacket(const FSendBufferRef& SendBuffer) const;

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets() const;

	virtual void Shutdown() override;

	void QuitGame();
	

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLoginState(ELoginState NewState);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELoginState LoginState = ELoginState::Login;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsConnected = false;

private:
	FSocket* Socket = nullptr;
	FString IpAddress = TEXT("127.0.0.1");
	int16 LoginPort = 7777;
	int16 GamePort = 7778;

	TSharedPtr<FPacketSession> Session;
};
