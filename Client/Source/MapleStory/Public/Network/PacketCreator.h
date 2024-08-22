// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapleStory.h"
#include "game_protocol.pb.h"
#include "Data/Enum/EKeyCode.h"
#include "Data/Enum/EKeyType.h"


class MAPLESTORY_API FPacketCreator {
public:
	static FSendBufferRef GetLoginRequest(const FString& Username, const FString& Password);
	static FSendBufferRef GetCharacterListRequest();
	static FSendBufferRef GetSelectCharacterRequest(int32 CharacterId);
	static FSendBufferRef GetDeleteCharacterRequest(int32 CharacterId);
	static FSendBufferRef GetCreateCharacterRequest(const FString& Name, int32 Avatar);

	static FSendBufferRef GetClientEnterRequest(int32 CharacterId, int32 AuthKey);
	static FSendBufferRef GetClientMove(float X, float Y, bool bIsRight, protocol::PlayerAnimation AnimationType);

	static FSendBufferRef GetChangeKeySetting(EKeyCode KeyCode, EKeyType KeyType, int32 ItemId = 0, int32 SkillId = 0);
};
