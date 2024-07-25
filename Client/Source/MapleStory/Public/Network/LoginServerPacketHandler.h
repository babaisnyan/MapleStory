#pragma once

#include "MapleStory.h"
#include "PacketHeader.h"
#include "login_protocol.pb.h"

class UMapleGameInstance;
using FPacketHandlerFunction = TFunction<bool(FPacketSessionRef&, uint8*, int32)>;

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
  PKT_LOGINSERVERCHAT = 2010,
};

bool HandleLoginInvalid(FPacketSessionRef& Session, uint8* Buffer, const int32 Len);
bool HandleLoginServerLogin(const FPacketSessionRef& Session, const protocol::LoginServerLogin& Packet);
bool HandleLoginServerCharacterList(const FPacketSessionRef& Session, const protocol::LoginServerCharacterList& Packet);
bool HandleLoginServerDeleteCharacter(const FPacketSessionRef& Session, const protocol::LoginServerDeleteCharacter& Packet);
bool HandleLoginServerCreateCharacter(const FPacketSessionRef& Session, const protocol::LoginServerCreateCharacter& Packet);
bool HandleLoginServerCharSelectResult(const FPacketSessionRef& Session, const protocol::LoginServerCharSelectResult& Packet);
bool HandleLoginServerChat(const FPacketSessionRef& Session, const protocol::LoginServerChat& Packet);

class FLoginServerPacketHandler {
public:
  static void Init(const TObjectPtr<UMapleGameInstance>& Instance) {
    FLoginServerPacketHandler::GameInstance = Instance;

    for (uint16 i = 0; i < UINT16_MAX; ++i) {
      PacketHandlers[i] = HandleLoginInvalid;
    }

    PacketHandlers[PKT_LOGINSERVERLOGIN] = [](FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
      return HandlePacket<protocol::LoginServerLogin>(HandleLoginServerLogin, Session, Buffer, Len);
    };
    PacketHandlers[PKT_LOGINSERVERCHARACTERLIST] = [](FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
      return HandlePacket<protocol::LoginServerCharacterList>(HandleLoginServerCharacterList, Session, Buffer, Len);
    };
    PacketHandlers[PKT_LOGINSERVERDELETECHARACTER] = [](FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
      return HandlePacket<protocol::LoginServerDeleteCharacter>(HandleLoginServerDeleteCharacter, Session, Buffer, Len);
    };
    PacketHandlers[PKT_LOGINSERVERCREATECHARACTER] = [](FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
      return HandlePacket<protocol::LoginServerCreateCharacter>(HandleLoginServerCreateCharacter, Session, Buffer, Len);
    };
    PacketHandlers[PKT_LOGINSERVERCHARSELECTRESULT] = [](FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
      return HandlePacket<protocol::LoginServerCharSelectResult>(HandleLoginServerCharSelectResult, Session, Buffer, Len);
    };
    PacketHandlers[PKT_LOGINSERVERCHAT] = [](FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
      return HandlePacket<protocol::LoginServerChat>(HandleLoginServerChat, Session, Buffer, Len);
    };
  }

  static bool HandlePacket(FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
    const FPacketHeader* Header = reinterpret_cast<FPacketHeader*>(Buffer);

    if (Header->PacketId >= UINT16_MAX) {
      return false;
    }

    return PacketHandlers[Header->PacketId](Session, Buffer, Len);
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
  static bool HandlePacket(Handler PacketHandler, FPacketSessionRef& Session, uint8* Buffer, const int32 Len) {
    PacketType Packet;

    if (!Packet.ParseFromArray(Buffer + sizeof(FPacketHeader), Len - sizeof(FPacketHeader))) {
      return false;
    }

    return PacketHandler(Session, Packet);
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

public:
  inline static TObjectPtr<UMapleGameInstance> GameInstance = nullptr;
  
private:
  inline static FPacketHandlerFunction PacketHandlers[UINT16_MAX] = {};
};