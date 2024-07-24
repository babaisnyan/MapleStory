#include "pch.h"
#include "login_handler.h"

#include "auth/migration_storage.h"

#include "network/login/login_client_packet_handler.h"
#include "network/login/login_session.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

#include "network/center/center_packet_creator.h"
#include "network/center/center_session_manager.h"
#include "network/login/login_packet_creator.h"

void LoginHandler::HandleLogin(PacketSessionRef session, protocol::LoginClientLogin request) {
  const LoginSessionRef login_session = std::static_pointer_cast<LoginSession>(session);
  protocol::LoginServerLogin response;
  response.set_result(protocol::LOGIN_RESULT_SERVER_ERROR);

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<2, 2> bind(*connection, L"{CALL dbo.spLogin(?, ?)}");

    const String username = utils::ConvertToWide(request.username()).value_or(L"");
    const String password = utils::ConvertToWide(request.password()).value_or(L"");
    bind.BindParam(0, username.c_str());
    bind.BindParam(1, password.c_str());

    int32_t result = 0;
    int32_t account_id = 0;
    bind.BindCol(0, result);
    bind.BindCol(1, account_id);

    if (bind.Execute()) {
      do {
        int16_t count = 0;
        bind.GetResultColumnCount(&count);

        if (count > 0 && bind.Fetch()) {
          switch (static_cast<protocol::LoginResult>(result + 1)) {
            case protocol::LOGIN_RESULT_SUCCESS:
              login_session->SetAccountId(account_id);
              [[fallthrough]];
            case protocol::LOGIN_RESULT_INVALID_USERNAME: [[fallthrough]];
            case protocol::LOGIN_RESULT_INVALID_PASSWORD: [[fallthrough]];
            case protocol::LOGIN_RESULT_ALREADY_LOGGED_IN:
              response.set_result(static_cast<protocol::LoginResult>(result + 1));
              break;
          }
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  SendPacket(login_session, response);
}

void LoginHandler::HandleCharacterList(PacketSessionRef session, protocol::LoginClientRequestCharacterList request) {
  const LoginSessionRef login_session = std::static_pointer_cast<LoginSession>(session);
  protocol::LoginServerCharacterList response;

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 9> bind(*connection, L"{CALL dbo.spGetLoginCharacters(?)}");

    int32_t account_id = login_session->GetAccountId();
    bind.BindParam(0, account_id);

    int32_t id;
    bind.BindCol(0, id);
    wchar_t name[50];
    bind.BindCol(1, name);
    int16_t level;
    bind.BindCol(2, level);
    int8_t type;
    bind.BindCol(3, type);
    int16_t job;
    bind.BindCol(4, job);
    int16_t str;
    bind.BindCol(5, str);
    int16_t dex;
    bind.BindCol(6, dex);
    int16_t luk;
    bind.BindCol(7, luk);
    int16_t int_;
    bind.BindCol(8, int_);

    if (bind.Execute()) {
      login_session->ClearCharacters();

      do {
        int16_t count = 0;
        bind.GetResultColumnCount(&count);
        if (count <= 0) continue;

        while (bind.Fetch()) {
          std::optional<std::string> name_str = utils::ConvertToUtf8(name);
          if (!name_str.has_value()) continue;

          protocol::LoginCharacter* character = response.add_characters();
          auto login_character = std::make_shared<LoginCharacter>(id, name, level, type, job, str, dex, luk, int_);

          character->set_id(id);
          character->set_name(name_str.value());
          character->set_level(level);
          character->set_type(type);
          character->set_job(job);
          character->set_str(str);
          character->set_dex(dex);
          character->set_luk(luk);
          character->set_int_(int_);
          login_session->AddCharacter(login_character);
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  SendPacket(login_session, response);
}

void LoginHandler::HandleSelectCharacter(PacketSessionRef session, protocol::LoginClientSelectCharacter request) {
  const LoginSessionRef login_session = std::static_pointer_cast<LoginSession>(session);

  bool found = false;

  for (const auto& character : login_session->GetCharacterList()) {
    if (character->GetId() == request.character_id()) {
      found = true;
      break;
    }
  }

  if (!found) {
    login_session->Send(LoginPacketCreator::GetSelectCharFailedResponse());
    return;
  }

  const auto player_ip = utils::ConvertToUtf8(session->GetNetworkAddress().GetIpAddress());
  const auto sever_name = utils::ConvertToUtf8(L"Game01"); // TODO: 설정으로 빼기

  if (!player_ip.has_value() || !sever_name.has_value()) {
    login_session->Send(LoginPacketCreator::GetSelectCharFailedResponse());
    return;
  }

  const auto migration_request = CenterPacketCreator::GetMigrationRequest(request.character_id(), player_ip.value(), sever_name.value());
  CenterSessionManager::GetInstance().Send(migration_request);

  const std::weak_ptr weak_session = login_session;
  MigrationStorage::GetInstance().Add(request.character_id(), weak_session);
}

void LoginHandler::HandleDeleteCharacter(PacketSessionRef session, protocol::LoginClientDeleteCharacter request) {
  const LoginSessionRef login_session = std::static_pointer_cast<LoginSession>(session);
  protocol::LoginServerDeleteCharacter response;
  response.set_success(false);
  response.set_character_id(request.character_id());

  bool found = false;

  for (const auto& character : login_session->GetCharacterList()) {
    if (character->GetId() == request.character_id()) {
      found = true;
      login_session->RemoveCharacter(character);
      break;
    }
  }

  if (found) {
    if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
      DbBind<1, 1> bind(*connection, L"{CALL dbo.spDeleteCharacter(?)}");

      int32_t character_id = request.character_id();
      bind.BindParam(0, character_id);

      int32_t result = 0;
      bind.BindCol(0, result);

      if (bind.Execute()) {
        do {
          int16_t count = 0;
          bind.GetResultColumnCount(&count);

          if (count > 0 && bind.Fetch()) {
            if (result == 1) {
              response.set_success(true);
            }
          }
        } while (bind.GetMoreResult() != SQL_NO_DATA);
      }

      DbConnectionPool::GetInstance().ReleaseConnection(connection);
    }
  }

  SendPacket(login_session, response);
}

void LoginHandler::HandleCreateCharacter(PacketSessionRef session, protocol::LoginClientCreateCharacter request) {
  const LoginSessionRef login_session = std::static_pointer_cast<LoginSession>(session);
  protocol::LoginServerCreateCharacter response;
  response.set_result(protocol::CREATE_CHAR_RESULT_SERVER_ERROR);

  if (request.type() < 1 || request.type() > 3) {
    login_session->Disconnect(L"Invalid character type");
    return;
  }

  const auto name = utils::ConvertToWide(request.name());

  if (!name.has_value()) {
    response.set_result(protocol::CREATE_CHAR_RESULT_INVALID_NAME);
    SendPacket(login_session, response);
    return;
  }

  if (name->size() < 2 || name->size() > 8) {
    response.set_result(protocol::CREATE_CHAR_RESULT_INVALID_NAME);
    SendPacket(login_session, response);
    return;
  }

  if (login_session->GetCharacterCount() >= 6) {
    response.set_result(protocol::CREATE_CHAR_RESULT_NO_SLOTS);
    SendPacket(login_session, response);
    return;
  }

  for (const auto& blocked_name : _forbidden_names) {
    if (name->find(blocked_name) != std::wstring::npos) {
      response.set_result(protocol::CREATE_CHAR_RESULT_INVALID_NAME);
      SendPacket(login_session, response);
      return;
    }
  }

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<3, 2> bind(*connection, L"{CALL dbo.spCreateCharacter(?, ?, ?)}");

    int32_t account_id = login_session->GetAccountId();
    int32_t type = request.type();
    bind.BindParam(0, account_id);
    bind.BindParam(1, name.value().c_str());
    bind.BindParam(2, type);

    int32_t result = 0;
    int32_t character_id = 0;
    bind.BindCol(0, result);
    bind.BindCol(1, character_id);

    if (bind.Execute()) {
      do {
        int16_t count = 0;
        bind.GetResultColumnCount(&count);
        if (count <= 0 || !bind.Fetch()) {}
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    switch (result) {
      case 1: {
        const auto character = LoadSingleCharacter(character_id);
        auto login_character = response.mutable_character();

        if (character.has_value()) {
          response.set_result(protocol::CREATE_CHAR_RESULT_SUCCESS);
          login_character->set_id(character.value()->GetId());
          login_character->set_name(request.name());
          login_character->set_level(character.value()->GetLevel());
          login_character->set_type(type);
          login_character->set_job(character.value()->GetJob());
          login_character->set_str(character.value()->GetStr());
          login_character->set_dex(character.value()->GetDex());
          login_character->set_luk(character.value()->GetLuk());
          login_character->set_int_(character.value()->GetInt());

          login_session->AddCharacter(character.value());
        }
      }
      break;
      case 2:
        response.set_result(protocol::CREATE_CHAR_RESULT_NAME_TAKEN);
        break;
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }


  SendPacket(login_session, response);
}

std::optional<std::shared_ptr<LoginCharacter>> LoginHandler::LoadSingleCharacter(int32_t character_id) {
  std::optional<std::shared_ptr<LoginCharacter>> ret = std::nullopt;

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 8> bind(*connection, L"{CALL dbo.spGetLoginCharacter(?)}");

    bind.BindParam(0, character_id);

    wchar_t name[50];
    bind.BindCol(0, name);
    int16_t level;
    bind.BindCol(1, level);
    int8_t type;
    bind.BindCol(2, type);
    int16_t job;
    bind.BindCol(3, job);
    int16_t str;
    bind.BindCol(4, str);
    int16_t dex;
    bind.BindCol(5, dex);
    int16_t luk;
    bind.BindCol(6, luk);
    int16_t int_;
    bind.BindCol(7, int_);

    if (bind.Execute()) {
      do {
        int16_t count = 0;
        bind.GetResultColumnCount(&count);

        if (count > 0 && bind.Fetch()) {
          ret = std::make_shared<LoginCharacter>(character_id, name, level, type, job, str, dex, luk, int_);
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return ret;
}
