#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"

class FPacketSession;
class FSocket;

class MAPLESTORY_API FRecvWorker final : public FRunnable {
public:
	explicit FRecvWorker(FSocket* Socket, TSharedPtr<FPacketSession> Session);

	virtual bool Init() override;
	virtual uint32 Run() override;

	void Destroy();

private:
	bool ReceivePacket(TArray<uint8>& OutPacket);
	bool ReceiveDesiredBytes(uint8* Buffer, int32 Size);

protected:
	FSocket* Socket = nullptr;
	FRunnableThread* Thread = nullptr;
	bool bRunning = true;

	TWeakPtr<FPacketSession> SessionRef;
};

class MAPLESTORY_API FSendWorker final : public FRunnable {
public:
	explicit FSendWorker(FSocket* Socket, TSharedPtr<class FPacketSession> Session);

	virtual bool Init() override;
	virtual uint32 Run() override;

	bool SendPacket(const FSendBufferRef& SendBuffer) const;
	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size) const;

protected:
	FSocket* Socket = nullptr;
	FRunnableThread* Thread = nullptr;
	bool bRunning = true;

	TWeakPtr<FPacketSession> SessionRef;
};
