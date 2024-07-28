// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"


class MAPLESTORY_API FPacketCreator {
public:
	static FSendBufferRef GetLoginRequest(const FString& Username, const FString& Password);
	static FSendBufferRef GetCharacterListRequest();
	static FSendBufferRef GetSelectCharacterRequest(int32 CharacterId);
	static FSendBufferRef GetDeleteCharacterRequest(int32 CharacterId);
	static FSendBufferRef GetCreateCharacterRequest(const FString& Name, int32 Avatar);

	static FSendBufferRef GetClientEnterRequest(int32 CharacterId, int32 AuthKey);
};
