#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"

struct MAPLESTORY_API FPacketHeader {
	FPacketHeader() : PacketSize(0), PacketId(0) {}
	FPacketHeader(const uint16 Size, const uint16 Id) : PacketSize(Size), PacketId(Id) {}

	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& PacketHeader) {
		Ar << PacketHeader.PacketSize;
		Ar << PacketHeader.PacketId;
		return Ar;
	}

	uint16 PacketSize;
	uint16 PacketId;
};
