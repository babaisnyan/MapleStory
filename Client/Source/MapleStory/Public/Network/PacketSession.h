// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"

class FSocket;
class FRecvWorker;
class FSendWorker;

class MAPLESTORY_API FPacketSession : public TSharedFromThis<FPacketSession> {
public:
	explicit FPacketSession(FSocket* Socket);
	~FPacketSession();

public:
	void Run();
	void Disconnect();

public:
	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(const FSendBufferRef& SendBuffer);

public:
	void EnqueueRecvPacket(const TArray<uint8>& Packet);

	bool DequeueSendPacket(FSendBufferRef& SendBuffer) {
		if (SendPacketQueue.IsEmpty()) return false;
		
		return SendPacketQueue.Dequeue(SendBuffer);
	}

private:
	FSocket* Socket;

	TSharedPtr<FRecvWorker> RecvWorkerThread;
	TSharedPtr<FSendWorker> SendWorkerThread;

	TQueue<TArray<uint8>> RecvPacketQueue;
	TQueue<FSendBufferRef> SendPacketQueue;
};