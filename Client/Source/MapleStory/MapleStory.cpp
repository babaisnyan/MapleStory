// Copyright Epic Games, Inc. All Rights Reserved.

#include "MapleStory.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, MapleStory, "MapleStory");

FSendBuffer::FSendBuffer(const int32 BufferSize) {
	Buffer.SetNum(BufferSize);
}

void FSendBuffer::Copy(const void* Data, const int32 Size) {
	FMemory::Memcpy(Buffer.GetData(), Data, Size);
	WriteSize = Size; 
}

void FSendBuffer::Close(const uint32 Size) {
	WriteSize = Size;
}
