#include "pch.h"
#include "center_handler.h"

#include "data/server_info.h"

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

void CenterHandler::HandleServerMigrationRequest(PacketSessionRef session, protocol::CenterClientMigrationRequest request) {
  const CenterSessionRef center_session = std::static_pointer_cast<CenterSession>(session);
  CenterServerMigrationResponse response;
  response.set_character_id(request.character_id());
  response.set_success(false);

  const auto server_name = utils::ConvertToWide(request.server_name());

  if (!server_name.has_value()) {
    SendResponse(center_session, response);
    return;
  }

  const auto server_info = CenterSessionManager::GetInstance().GetServerInfo(server_name.value());

  if (!server_info.has_value()) {
    SendResponse(center_session, response);
    return;
  }

  const auto server_ip = utils::ConvertToUtf8(server_info.value()->ip);

  if (!server_ip.has_value()) {
    SendResponse(center_session, response);
    return;
  }

  auto* info = response.mutable_server();
  info->set_name(request.server_name());
  info->set_ip(server_ip.value());
  info->set_port(server_info.value()->port);
  response.set_success(true);

  SendResponse(center_session, response);

  //TODO: 게임서버에 정보 등록
}
