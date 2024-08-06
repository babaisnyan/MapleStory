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
		PKT_GAMESERVERCHANGEMAP = 3003,
	};

	static bool HandleGameInvalid(const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len);
	static bool HandleGameServerEnter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerEnter& Packet);
	static bool HandleGameServerAddPlayer(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerAddPlayer& Packet);
	static bool HandleGameServerChangeMap(const TObjectPtr<UTCPClientComponent>& Client, const protocol::GameServerChangeMap& Packet);

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
		PacketHandlers[PKT_GAMESERVERCHANGEMAP] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::GameServerChangeMap>(HandleGameServerChangeMap, Client, Buffer, Len);
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