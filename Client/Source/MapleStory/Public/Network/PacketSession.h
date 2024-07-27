// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"
#include "Data/Enum/EServerType.h"

class FSocket;
class FRecvWorker;
class FSendWorker;

class MAPLESTORY_API FPacketSession : public TSharedFromThis<FPacketSession> {
public:
	explicit FPacketSession(FSocket* Socket, EServerType Type);

public:
	void Run();
	void Disconnect();

public:
	void HandleRecvPackets();
	void SendPacket(const FSendBufferRef& SendBuffer) const;

public:
	void EnqueueRecvPacket(const TArray<uint8>& Packet) const;

	bool DequeueSendPacket(FSendBufferRef& SendBuffer) const {
		if (SendPacketQueue->IsEmpty()) return false;

		return SendPacketQueue->Dequeue(SendBuffer);
	}

private:
	FSocket* Socket;
	EServerType ServerType;
	bool bRunning = true;
	
	TSharedPtr<FRecvWorker> RecvWorkerThread;
	TSharedPtr<FSendWorker> SendWorkerThread;

	TUniquePtr<TQueue<TArray<uint8>>> RecvPacketQueue;
	TUniquePtr<TQueue<FSendBufferRef>> SendPacketQueue;
};
