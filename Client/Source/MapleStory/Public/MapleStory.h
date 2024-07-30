#pragma once

#include "CoreMinimal.h"

class FSendBuffer : public TSharedFromThis<FSendBuffer> {
public:
	explicit FSendBuffer(int32 BufferSize);
	~FSendBuffer() = default;

	uint8* GetBuffer() { return Buffer.GetData(); }
	int32 GetWriteSize() const { return WriteSize; }
	int32 GetCapacity() const { return Buffer.Num(); }

	void Copy(const void* Data, int32 Size);
	void Close(uint32 Size);
private:
	TArray<uint8> Buffer;
	int32 WriteSize = 0;
};

#define DECLARE_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>;

DECLARE_SHARED_PTR(FSession);
DECLARE_SHARED_PTR(FPacketSession);
DECLARE_SHARED_PTR(FSendBuffer);

#define SEND_LOGIN_PACKET(Packet) \
	FSendBufferRef SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet); \
	Cast<UMapleGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);

#define SEND_GAME_PACKET(Packet) \
	FSendBufferRef SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet); \
	Cast<UMapleGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);