// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"

#include "Engine/GameInstance.h"
#include "Public/Data/Enum/ELoginState.h"

#include "MapleGameInstance.generated.h"

class FPacketSession;

UCLASS()
class MAPLESTORY_API UMapleGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToLoginServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromLoginServer();
	
	void SendPacket(FSendBufferRef SendBuffer);
	
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets() const;

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLoginState(ELoginState NewState);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ELoginState LoginState = ELoginState::Login;

private:
	FSocket* Socket = nullptr;
	FString IpAddress = TEXT("127.0.0.1");
	int16 LoginPort = 7777;
	int16 GamePort = 7778;

	TSharedPtr<FPacketSession> Session;
};
