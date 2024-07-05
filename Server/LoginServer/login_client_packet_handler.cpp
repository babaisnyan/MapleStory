#include "pch.h"
#include "login_client_packet_handler.h"

#include "log/console_log.h"

bool HandleLoginInvalid(PacketSessionRef& session, std::byte* buffer, const int32_t len) {
  return false;
}

bool HandleLoginClientLogin(const PacketSessionRef& session, const protocol::LoginClientLogin& packet) {
  protocol::LoginServerLogin response;

  ConsoleLog::GetInstance().WriteStdOut(Color::kGreen, L"LoginClientLogin: %s, %s\n", packet.username().c_str(), packet.password().c_str());

  if (packet.username() == "test" && packet.password() == "test") {
     response.set_success(true);
  } else {
     response.set_success(false);
  }

  const auto send_buffer = LoginClientPacketHandler::MakeSendBuffer(response);
  session->Send(send_buffer);

  return true;
}
