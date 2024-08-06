#pragma once

#include "MapleGameInstance.h"
#include "MapleStory.h"
#include "PacketHeader.h"
#include "login_protocol.pb.h"

class UTCPClientComponent;
class UMapleGameInstance;

class FLoginServerPacketHandler {
	using FPacketHandlerFunction = TFunction<bool(TObjectPtr<UTCPClientComponent>, const uint8*, int32)>;

	enum : uint16 {
		PKT_LOGINCLIENTLOGIN = 2000,
		PKT_LOGINSERVERLOGIN = 2001,
		PKT_LOGINCLIENTREQUESTCHARACTERLIST = 2002,
		PKT_LOGINSERVERCHARACTERLIST = 2003,
		PKT_LOGINCLIENTSELECTCHARACTER = 2004,
		PKT_LOGINCLIENTDELETECHARACTER = 2005,
		PKT_LOGINSERVERDELETECHARACTER = 2006,
		PKT_LOGINCLIENTCREATECHARACTER = 2007,
		PKT_LOGINSERVERCREATECHARACTER = 2008,
		PKT_LOGINSERVERCHARSELECTRESULT = 2009,
	};

	static bool HandleLoginInvalid(const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len);
	static bool HandleLoginServerLogin(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerLogin& Packet);
	static bool HandleLoginServerCharacterList(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerCharacterList& Packet);
	static bool HandleLoginServerDeleteCharacter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerDeleteCharacter& Packet);
	static bool HandleLoginServerCreateCharacter(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerCreateCharacter& Packet);
	static bool HandleLoginServerCharSelectResult(const TObjectPtr<UTCPClientComponent>& Client, const protocol::LoginServerCharSelectResult& Packet);

public:
	static void Init(const TWeakObjectPtr<UMapleGameInstance>& Instance) {
		GameInstance = Instance;

		for (uint16 i = 0; i < UINT16_MAX; ++i) {
			PacketHandlers[i] = HandleLoginInvalid;
		}

		PacketHandlers[PKT_LOGINSERVERLOGIN] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::LoginServerLogin>(HandleLoginServerLogin, Client, Buffer, Len);
		};
		PacketHandlers[PKT_LOGINSERVERCHARACTERLIST] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::LoginServerCharacterList>(HandleLoginServerCharacterList, Client, Buffer, Len);
		};
		PacketHandlers[PKT_LOGINSERVERDELETECHARACTER] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::LoginServerDeleteCharacter>(HandleLoginServerDeleteCharacter, Client, Buffer, Len);
		};
		PacketHandlers[PKT_LOGINSERVERCREATECHARACTER] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::LoginServerCreateCharacter>(HandleLoginServerCreateCharacter, Client, Buffer, Len);
		};
		PacketHandlers[PKT_LOGINSERVERCHARSELECTRESULT] = [](const TObjectPtr<UTCPClientComponent>& Client, const uint8* Buffer, const int32 Len) {
			return HandlePacket<protocol::LoginServerCharSelectResult>(HandleLoginServerCharSelectResult, Client, Buffer, Len);
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

	static FSendBufferRef MakeSendBuffer(const protocol::LoginClientLogin& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_LOGINCLIENTLOGIN); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::LoginClientRequestCharacterList& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_LOGINCLIENTREQUESTCHARACTERLIST); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::LoginClientSelectCharacter& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_LOGINCLIENTSELECTCHARACTER); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::LoginClientDeleteCharacter& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_LOGINCLIENTDELETECHARACTER); 
	}
	static FSendBufferRef MakeSendBuffer(const protocol::LoginClientCreateCharacter& Packet) { 
		return MakeSendBufferInternal(Packet, PKT_LOGINCLIENTCREATECHARACTER); 
	}

private:
	template <typename PacketType, typename Handler>
	static bool HandlePacket(Handler PacketHandler, TObjectPtr<UTCPClientComponent> Client, const uint8* Buffer, const int32 Len) {
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