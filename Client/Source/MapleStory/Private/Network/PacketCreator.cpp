// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/PacketCreator.h"

#include "login_protocol.pb.h"
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
