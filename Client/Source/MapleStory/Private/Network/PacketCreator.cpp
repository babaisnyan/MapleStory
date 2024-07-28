// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketCreator.h"

#include "game_protocol.pb.h"
#include "login_protocol.pb.h"
#include "Network/GameServerPacketHandler.h"
#include "Network/LoginServerPacketHandler.h"


using namespace protocol;

FSendBufferRef FPacketCreator::GetLoginRequest(const FString& Username, const FString& Password) {
	LoginClientLogin Packet;
	Packet.set_username(TCHAR_TO_UTF8(*Username));
	Packet.set_password(TCHAR_TO_UTF8(*Password));

	const auto SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetCharacterListRequest() {
	const LoginClientRequestCharacterList Packet;

	const auto SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetSelectCharacterRequest(const int32 CharacterId) {
	LoginClientSelectCharacter Packet;
	Packet.set_character_id(CharacterId);

	const auto SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetDeleteCharacterRequest(const int32 CharacterId) {
	LoginClientDeleteCharacter Packet;
	Packet.set_character_id(CharacterId);

	const auto SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetCreateCharacterRequest(const FString& Name, const int32 Avatar) {
	LoginClientCreateCharacter Packet;
	Packet.set_name(TCHAR_TO_UTF8(*Name));
	Packet.set_type(Avatar);

	const auto SendBuffer = FLoginServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetClientEnterRequest(const int32 CharacterId, const int32 AuthKey) {
	GameClientEnter Packet;
	Packet.set_character_id(CharacterId);
	Packet.set_auth_key(AuthKey);

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}
