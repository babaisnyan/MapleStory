#include "pch.h"
#include "center_handler.h"

#include "data/server_info.h"

#include "migration/migration_storage.h"

#include "network/center_client_packet_handler.h"
#include "network/center_session.h"
#include "network/center_session_manager.h"
#include "network/packet_creator.h"

using namespace protocol;

void CenterHandler::HandleServerRegisterRequest(PacketSessionRef session, CenterClientRegisterRequest request) {
  const auto center_session = std::static_pointer_cast<CenterSession>(session);
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
  const auto center_session = std::static_pointer_cast<CenterSession>(session);
  CenterServerMigrationResponse response;
  response.set_character_id(request.character_id());
  response.set_success(false);

  const auto server_name = utils::ConvertToWide(request.server_name());

  if (!server_name.has_value()) {
    SendResponse(center_session, response);
    return;
  }

  const auto game_server = CenterSessionManager::GetInstance().Get(server_name.value());

  if (!game_server.has_value()) {
    SendResponse(center_session, response);
    return;
  }

  const auto migration_data = std::make_shared<MigrationData>(request.character_id(), center_session, game_server.value());
  MigrationStorage::GetInstance().Add(request.character_id(), migration_data);

  const auto send_buffer = PacketCreator::GetMigrationRequest(request.character_id());
  game_server.value()->Send(send_buffer);
}

void CenterHandler::HandleServerMigrationResponse(PacketSessionRef session, CenterClientMigrationResponse response) {
  const auto success = response.success();
  const auto character_id = response.character_id();
  const auto migration_data = MigrationStorage::GetInstance().Find(character_id);

  if (!migration_data.has_value()) {
    return;
  }

  const auto from_server = migration_data.value()->from_session.lock();
  const auto target_server = migration_data.value()->to_session.lock();

  if (!from_server || !target_server) {
    MigrationStorage::GetInstance().Remove(character_id);
    return;
  }

  if (success) {
    const auto name = utils::ConvertToUtf8(target_server->GetServerInfo()->name);
    const auto ip = utils::ConvertToUtf8(target_server->GetServerInfo()->ip);

    if (!name.has_value() || !ip.has_value()) {
      MigrationStorage::GetInstance().Remove(character_id);
      return;
    }

    const auto send_buffer = PacketCreator::GetMigrationSuccessResponse(character_id, response.auth_key(), name.value(), ip.value(), target_server->GetServerInfo()->port);
    from_server->Send(send_buffer);
  } else {
    const auto send_buffer = PacketCreator::GetMigrationFailedResponse(character_id);
    from_server->Send(send_buffer);
  }
}
