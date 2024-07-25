#pragma once
#include "job/job_queue.h"

#include "network/protocol/login_protocol.pb.h"

#include "network/login/login_client_packet_handler.h"
#include "network/login/login_session.h"

namespace login {
  class LoginHandler final : public JobQueue {
  public:
    void HandleLogin(PacketSessionRef session, protocol::LoginClientLogin request);
    void HandleCharacterList(PacketSessionRef session, protocol::LoginClientRequestCharacterList request);
    void HandleSelectCharacter(PacketSessionRef session, protocol::LoginClientSelectCharacter request);
    void HandleDeleteCharacter(PacketSessionRef session, protocol::LoginClientDeleteCharacter request);
    void HandleCreateCharacter(PacketSessionRef session, protocol::LoginClientCreateCharacter request);

  private:
    std::optional<std::shared_ptr<LoginCharacter>> LoadSingleCharacter(int32_t character_id);

    template <typename T> requires std::is_base_of_v<google::protobuf::Message, T>
    void SendPacket(const LoginSessionRef& session, T& message) {
      session->Send(LoginClientPacketHandler::MakeSendBuffer(message));
    }

  private:
    static constexpr std::array _forbidden_names = {
      L"admin", L"gm", L"master", L"moderator", L"dev", L"test", L"테스트", L"운영자", L"관리자"
    };
  };
}
