// ReSharper disable CppInconsistentNaming

#pragma once
#include "protocol/center_protocol.pb.h"

using PacketHandler = std::function<bool(PacketSessionRef&, std::byte*, int32_t)>;

enum : uint16_t {
  PKT_CENTERSERVERHANDSHAKE = 1000,
  PKT_CENTERCLIENTREGISTERREQUEST = 1001,
  PKT_CENTERSERVERREGISTERRESPONSE = 1002,
};

bool HandleCenterInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len);
bool HandleCenterClientRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& packet);

class CenterClientPacketHandler {
public:
  static void Init() {
    _packet_handler_map.reserve(UINT16_MAX);

    for (uint16_t i = 0; i < UINT16_MAX; i++) {
      _packet_handler_map[i] = HandleCenterInvalid;
    }


    _packet_handler_map[PKT_CENTERCLIENTREGISTERREQUEST] = [](PacketSessionRef& session, std::byte* buffer, const int32_t len) {
      return HandlePacket<protocol::CenterClientRegisterRequest>(HandleCenterClientRegisterRequest, session, buffer, len);
    };
  }

  static bool HandlePacket(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
    const PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

    if (!_packet_handler_map.contains(header->id)) {
      return false;
    }

    return _packet_handler_map[header->id](session, buffer, len);
  }


  static SendBufferRef MakeSendBuffer(const protocol::CenterServerHandshake& packet) { 
    return MakeSendBufferInternal(packet, PKT_CENTERSERVERHANDSHAKE); 
  }
  static SendBufferRef MakeSendBuffer(const protocol::CenterServerRegisterResponse& packet) { 
    return MakeSendBufferInternal(packet, PKT_CENTERSERVERREGISTERRESPONSE); 
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