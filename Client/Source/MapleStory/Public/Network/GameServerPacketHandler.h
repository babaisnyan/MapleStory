#pragma once

#include "MapleGameInstance.h"
#include "MapleStory.h"
#include "PacketHeader.h"
#include "game_protocol.pb.h"

class UTCPClientComponent;
class UMapleGameInstance;

class FGameServerPacketHandler {
	using FPacketHandlerFunction = TFunction<bool(TObjectPtr<UTCPClientComponent>, const uint8*, int32)>;

	enum : uint16 {
		PKT_GAMECLIENTENTER = 3000,
		PKT_GAMESERVERENTER = 3001,
		PKT_GAMESERVERADDPLAYER = 3002,
		PKT_GAMESERVERADDMONSTER = 3003,
		PKT_GAMESERVERREMOVEOBJECT = 3004,
		PKT_GAMESERVERCHANGEMAP = 3005,
		PKT_GAMECLIENTPLAYERMOVE = 3006,
		PKT_GAMESERVERPLAYERMOVE = 3007,
		PKT_GAMECLIENTCHANGEKEYSETTING = 3008,
		PKT_GAMESERVERMOBMOVE = 3009,
		PKT_GAMESERVERMOBATTACK = 3010,
		PKT_GAMESERVERPLAYERDAMAGE = 3011,
		PKT_GAMESERVERMOBDAMAGE = 3012,
		PKT_GAMECLIENTCHAT = 3013,
		PKT_GAMESERVERCHAT = 3014,
		PKT_GAMESERVERPLAYERDEAD = 3015,
		PKT_GAMECLIENTREVIVE = 3016,
		PKT_GAMESERVERREVIVE = 3017,
		PKT_GAMESERVERTELEPORTPLAYER = 3018,
		PKT_GAMESERVERUPDATEPLAYERSTAT = 3019,
		PKT_GAMECLIENTATTACK = 3020,
		PKT_GAMESERVERATTACK = 3021,
		PKT_GAMESERVERPLAYERLEVELUP = 3022,
		PKT_GAMESERVERADDEXPMESSAGE = 3023,
	};

	static bool HandleGameInvalid(const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len);
	static bool HandleGameServerEnter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerEnter& Packet);
	static bool HandleGameServerAddPlayer(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddPlayer& Packet);
	static bool HandleGameServerAddMonster(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddMonster& Packet);
	static bool HandleGameServerRemoveObject(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerRemoveObject& Packet);
	static bool HandleGameServerChangeMap(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChangeMap& Packet);
	static bool HandleGameServerPlayerMove(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerMove& Packet);
	static bool HandleGameServerMobMove(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobMove& Packet);
	static bool HandleGameServerMobAttack(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobAttack& Packet);
	static bool HandleGameServerPlayerDamage(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerDamage& Packet);
	static bool HandleGameServerMobDamage(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerMobDamage& Packet);
	static bool HandleGameServerChat(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChat& Packet);
	static bool HandleGameServerPlayerDead(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerDead& Packet);
	static bool HandleGameServerRevive(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerRevive& Packet);
	static bool HandleGameServerTeleportPlayer(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerTeleportPlayer& Packet);
	static bool HandleGameServerUpdatePlayerStat(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerUpdatePlayerStat& Packet);
	static bool HandleGameServerAttack(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAttack& Packet);
	static bool HandleGameServerPlayerLevelUp(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerPlayerLevelUp& Packet);
	static bool HandleGameServerAddExpMessage(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddExpMessage& Packet);

public:
	static void Init(const TWeakObjectPtr<UMapleGameInstance>& Instance) {
		GameInstance = Instance;

		for (uint16 i = 0; i < UINT16_MAX; ++i) {
			PacketHandlers[i] = HandleGameInvalid;
		}

		PacketHandlers[PKT_GAMESERVERENTER] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerEnter>(HandleGameServerEnter, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERADDPLAYER] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerAddPlayer>(HandleGameServerAddPlayer, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERADDMONSTER] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerAddMonster>(HandleGameServerAddMonster, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERREMOVEOBJECT] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerRemoveObject>(HandleGameServerRemoveObject, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERCHANGEMAP] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerChangeMap>(HandleGameServerChangeMap, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERPLAYERMOVE] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerPlayerMove>(HandleGameServerPlayerMove, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERMOBMOVE] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerMobMove>(HandleGameServerMobMove, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERMOBATTACK] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerMobAttack>(HandleGameServerMobAttack, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERPLAYERDAMAGE] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerPlayerDamage>(HandleGameServerPlayerDamage, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERMOBDAMAGE] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerMobDamage>(HandleGameServerMobDamage, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERCHAT] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerChat>(HandleGameServerChat, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERPLAYERDEAD] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerPlayerDead>(HandleGameServerPlayerDead, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERREVIVE] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerRevive>(HandleGameServerRevive, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERTELEPORTPLAYER] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerTeleportPlayer>(HandleGameServerTeleportPlayer, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERUPDATEPLAYERSTAT] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerUpdatePlayerStat>(HandleGameServerUpdatePlayerStat, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERATTACK] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerAttack>(HandleGameServerAttack, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERPLAYERLEVELUP] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerPlayerLevelUp>(HandleGameServerPlayerLevelUp, Client, Buffer, Len);
		};
		PacketHandlers[PKT_GAMESERVERADDEXPMESSAGE] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerAddExpMessage>(HandleGameServerAddExpMessage, Client, Buffer, Len);
		};
	}

	static bool HandlePacket(const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
		const FPacketHeader* Header = reinterpret_cast<const FPacketHeader*>(Buffer);

		if (Header->PacketId >= UINT16_MAX) {
			return false;
		}

		if (!GameInstance.IsValid()) {
			return false;
		}

		return PacketHandlers[Header->PacketId](Client, Buffer, Len);
	}

	static FSendBufferRef MakeSendBuffer(const protocol::GameClientEnter& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_GAMECLIENTENTER); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::GameClientPlayerMove& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_GAMECLIENTPLAYERMOVE); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::GameClientChangeKeySetting& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_GAMECLIENTCHANGEKEYSETTING); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::GameClientChat& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_GAMECLIENTCHAT); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::GameClientRevive& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_GAMECLIENTREVIVE); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::GameClientAttack& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_GAMECLIENTATTACK); 
	}

private:
	template <typename PacketType, typename Handler>
	static bool HandlePacket(Handler PacketHandler, const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
		PacketType Packet;

		if (!Packet.ParseFromArray(Buffer + sizeof(FPacketHeader), Len - sizeof(FPacketHeader))) {
			return false;
		}

		return PacketHandler(Client, Packet);
	}

	template <typename T>
	static FSendBufferRef MakeSendBufferInternal(const T& Packet, const uint16 PacketId) {
		const uint16 DataSize = static_cast<uint16>(Packet.ByteSizeLong());
		const uint16 PacketSize = DataSize + sizeof(FPacketHeader);
		FSendBufferRef SendBuffer = MakeShared<FSendBuffer>(PacketSize);

		FPacketHeader* Header = reinterpret_cast<FPacketHeader*>(SendBuffer->GetBuffer());
		Header->PacketSize = PacketSize;
		Header->PacketId = PacketId;
		Packet.SerializeToArray(&Header[1], DataSize);

		SendBuffer->Close(PacketSize);
		return SendBuffer;
	}

private:
	inline static FPacketHandlerFunction PacketHandlers[UINT16_MAX] = {};
	inline static TWeakObjectPtr<UMapleGameInstance> GameInstance = nullptr;
};