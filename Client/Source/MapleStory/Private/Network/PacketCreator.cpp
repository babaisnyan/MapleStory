

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

FSendBufferRef FPacketCreator::GetClientMove(const int32 X, const int32 Y, const bool bFlip, const PlayerAnimation AnimationType) {
	GameClientPlayerMove Packet;
	Packet.set_x(X);
	Packet.set_y(Y);
	Packet.set_flip(bFlip);
	Packet.set_animation(AnimationType);

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetChangeKeySetting(EKeyCode KeyCode, EKeyType KeyType, const int32 ItemId, const int32 SkillId) {
	GameClientChangeKeySetting Packet;
	KeySetting* KeySetting = Packet.mutable_key_setting();
	KeySetting->set_key_code(static_cast<protocol::KeyCode>(KeyCode));
	KeySetting->set_key_type(static_cast<protocol::KeyType>(KeyType));

	if (KeyType == EKeyType::Item) {
		KeySetting->set_item_id(ItemId);
	} else if (KeyType == EKeyType::Skill) {
		KeySetting->set_skill_id(SkillId);
	}

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetChat(const FString& Text) {
	GameClientChat Packet;
	Packet.set_message(TCHAR_TO_UTF8(*Text));

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetReviveRequest() {
	const GameClientRevive Packet;
	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetAttackRequest(const int64 MobId) {
	GameClientAttack Packet;
	Packet.set_target_id(MobId);
	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetMoveItemRequest(const int32 Type, const int32 From, const int32 To) {
	GameClientMoveInventory Packet;
	Packet.set_type(Type);
	Packet.set_src_pos(From);
	Packet.set_dest_pos(To);

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetUseItemRequest(const int32 Pos) {
	GameClientUseItem Packet;
	Packet.set_pos(Pos);

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetEquipItemRequest(const int32 From, const int32 To) {
	GameClientEquipItem Packet;
	Packet.set_src_pos(From);
	Packet.set_dest_pos(To);

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}

FSendBufferRef FPacketCreator::GetUnEquipItemRequest(const int32 From, const int32 To) {
	GameClientUnequipItem Packet;
	Packet.set_src_pos(From);
	Packet.set_dest_pos(To);

	const auto SendBuffer = FGameServerPacketHandler::MakeSendBuffer(Packet);
	return SendBuffer;
}
