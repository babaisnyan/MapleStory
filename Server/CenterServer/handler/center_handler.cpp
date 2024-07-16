#include "pch.h"
#include "center_handler.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

#include "network/center_client_packet_handler.h"
#include "network/center_session.h"
#include "network/center_session_manager.h"

using namespace protocol;

void CenterHandler::HandleServerRegisterRequest(PacketSessionRef session, CenterClientRegisterRequest request) {
  const CenterSessionRef center_session = std::static_pointer_cast<CenterSession>(session);
  CenterServerRegisterResponse response;
  response.set_result(SERVER_REGISTER_RESULT_ERROR);

  if (request.version() != constants::kVersion) {
    response.set_result(SERVER_REGISTER_RESULT_INVALID_VERSION);
    SendResponse(center_session, response);
    return;
  }

  if (request.type() == SERVER_TYPE_CENTER || request.type() == SERVER_TYPE_UNSPECIFIED) {
    response.set_result(SERVER_REGISTER_RESULT_INVALID_TYPE);
    SendResponse(center_session, response);
    return;
  }

  if (request.name().empty()) {
    response.set_result(SERVER_REGISTER_RESULT_INVALID_NAME);
    SendResponse(center_session, response);
    return;
  }

  const auto name = utils::ConvertToWide(request.name());
  const auto ip = utils::ConvertToWide(request.ip());

  if (!name.has_value()) {
    response.set_result(SERVER_REGISTER_RESULT_INVALID_NAME);
    SendResponse(center_session, response);
    return;
  }

  if (!ip.has_value()) {
    response.set_result(SERVER_REGISTER_RESULT_INVALID_IP);
    SendResponse(center_session, response);
    return;
  }

  if (request.port() <= 0) {
    response.set_result(SERVER_REGISTER_RESULT_INVALID_PORT);
    SendResponse(center_session, response);
    return;
  }

  if (CenterSessionManager::GetInstance().IsExist(name.value())) {
    response.set_result(SERVER_REGISTER_RESULT_ALREADY_REGISTERED);
    SendResponse(center_session, response);
    return;
  }

  if (CenterSessionManager::GetInstance().IsExist(ip.value(), request.port())) {
    response.set_result(SERVER_REGISTER_RESULT_ALREADY_REGISTERED);
    SendResponse(center_session, response);
    return;
  }

  center_session->RegisterServer(name.value(), ip.value(), request.port());
  response.set_result(SERVER_REGISTER_RESULT_SUCCESS);
  SendResponse(center_session, response);
}

void CenterHandler::SendResponse(const CenterSessionRef& session, const CenterServerRegisterResponse& response) {
  const auto buffer = CenterClientPacketHandler::MakeSendBuffer(response);
  session->Send(buffer);
}
