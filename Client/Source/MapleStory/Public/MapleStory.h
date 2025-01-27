#pragma once

#include "CoreMinimal.h"

class FSendBuffer : public TSharedFromThis<FSendBuffer> {
public:
	explicit FSendBuffer(int32 BufferSize);
	~FSendBuffer() = default;

	uint8* GetBuffer() { return Buffer.GetData(); }
	TArray<uint8>& GetBufferArray() { return Buffer; }
	int32 GetWriteSize() const { return WriteSize; }
	int32 GetCapacity() const { return Buffer.Num(); }

	void Copy(const void* Data, int32 Size);
	void Close(uint32 Size);
private:
	TArray<uint8> Buffer;
	int32 WriteSize = 0;
};

#define DECLARE_SHARED_PTR(name) using name##Ref = TSharedPtr<class name>;

DECLARE_SHARED_PTR(FSendBuffer);

#define SEND_PACKET(SendBuffer) Cast<UMapleGameInstance>(GWorld->GetGameInstance())->SendPacket(SendBuffer);
DECLARE_STATS_GROUP(TEXT("MapleStory"), STATGROUP_MapleStory, STATCAT_Advanced);