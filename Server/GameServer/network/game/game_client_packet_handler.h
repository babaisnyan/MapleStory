// ReSharper disable CppInconsistentNaming

#pragma once
#include "network/protocol/game_protocol.pb.h"
namespace game {
  using PacketHandler = std::function<bool(PacketSessionRef&, std::byte*, int32_t)>;

  enum : uint16_t {
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
  };

  bool HandleGameInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len);
  bool HandleGameClientEnter(const PacketSessionRef& session, const protocol::GameClientEnter& packet);
  bool HandleGameClientPlayerMove(const PacketSessionRef& session, const protocol::GameClientPlayerMove& packet);
  bool HandleGameClientChangeKeySetting(const PacketSessionRef& session, const protocol::GameClientChangeKeySetting& packet);
  bool HandleGameClientChat(const PacketSessionRef& session, const protocol::GameClientChat& packet);

  class GameClientPacketHandler {
  public:
    static void Init() {
      _packet_handler_map.reserve(UINT16_MAX);

      for (uint16_t i = 0; i < UINT16_MAX; i++) {
        _packet_handler_map[i] = HandleGameInvalid;
      }

  
      _packet_handler_map[PKT_GAMECLIENTENTER] = [](PacketSessionRef& session, std::byte* buffer, const int32_t len) {
        return HandlePacket<protocol::GameClientEnter>(HandleGameClientEnter, session, buffer, len);
      };
      _packet_handler_map[PKT_GAMECLIENTPLAYERMOVE] = [](PacketSessionRef& session, std::byte* buffer, const int32_t len) {
        return HandlePacket<protocol::GameClientPlayerMove>(HandleGameClientPlayerMove, session, buffer, len);
      };
      _packet_handler_map[PKT_GAMECLIENTCHANGEKEYSETTING] = [](PacketSessionRef& session, std::byte* buffer, const int32_t len) {
        return HandlePacket<protocol::GameClientChangeKeySetting>(HandleGameClientChangeKeySetting, session, buffer, len);
      };
      _packet_handler_map[PKT_GAMECLIENTCHAT] = [](PacketSessionRef& session, std::byte* buffer, const int32_t len) {
        return HandlePacket<protocol::GameClientChat>(HandleGameClientChat, session, buffer, len);
      };
    }

    static bool HandlePacket(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
      const PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

      if (!_packet_handler_map.contains(header->id)) {
        return false;
      }

      return _packet_handler_map[header->id](session, buffer, len);
    }

  
    static SendBufferRef MakeSendBuffer(const protocol::GameServerEnter& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERENTER); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerAddPlayer& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERADDPLAYER); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerAddMonster& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERADDMONSTER); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerRemoveObject& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERREMOVEOBJECT); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerChangeMap& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERCHANGEMAP); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerPlayerMove& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERPLAYERMOVE); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerMobMove& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERMOBMOVE); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerMobAttack& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERMOBATTACK); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerPlayerDamage& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERPLAYERDAMAGE); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerMobDamage& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERMOBDAMAGE); 
    }
    static SendBufferRef MakeSendBuffer(const protocol::GameServerChat& packet) { 
      return MakeSendBufferInternal(packet, PKT_GAMESERVERCHAT); 
    }

  private:
    template <typename PacketType, typename Handler>
    static bool HandlePacket(Handler handler, PacketSessionRef& session, std::byte* buffer, const int32_t len) {
      PacketType packet;

      if (!packet.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader))) {
        return false;
      }

      return handler(session, packet);
    }

    template <typename T>
    static SendBufferRef MakeSendBufferInternal(const T& packet, const uint16_t packet_id) {
      const uint16_t data_size = static_cast<uint16_t>(packet.ByteSizeLong());
      const uint16_t packet_size = data_size + sizeof(PacketHeader);
      SendBufferRef  send_buffer = SendBufferManager::GetInstance().Open(packet_size);

      PacketHeader* header = reinterpret_cast<PacketHeader*>(send_buffer->GetBuffer());
      header->size = packet_size;
      header->id = packet_id;

      ASSERT_CRASH(packet.SerializeToArray(&header[1], data_size));

      send_buffer->Close(packet_size);
      return send_buffer;
    }

  private:
    inline static std::unordered_map<uint16_t, PacketHandler> _packet_handler_map;
  };
}