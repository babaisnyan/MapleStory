#include "pch.h"
#include "login_handler.h"

#include "../network/login_client_packet_handler.h"
#include "../network/login_session.h"
#include "../network/login_session_manager.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

void LoginHandler::HandleLogin(PacketSessionRef session, protocol::LoginClientLogin request) {
  const LoginSessionRef      login_session = std::static_pointer_cast<LoginSession>(session);
  protocol::LoginServerLogin response;
  response.set_result(protocol::ServerError);

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<2, 2> bind(*connection, L"{CALL dbo.spLogin(?, ?)}");

    const String username = ConvertToWide(request.username()).value_or(L"");
    const String password = ConvertToWide(request.password()).value_or(L"");
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
          switch (static_cast<protocol::LoginResult>(result)) {
            case protocol::Success:
              login_session->SetAccountId(account_id);
              [[fallthrough]];
            case protocol::InvalidUsername: [[fallthrough]];
            case protocol::InvalidPassword: [[fallthrough]];
            case protocol::AlreadyLoggedIn:
              response.set_result(static_cast<protocol::LoginResult>(result));
              break;
          }
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  login_session->Send(LoginClientPacketHandler::MakeSendBuffer(response));
}

void LoginHandler::HandleCharacterList(PacketSessionRef session, protocol::LoginClientRequestCharacterList request) {
  const LoginSessionRef              login_session = std::static_pointer_cast<LoginSession>(session);
  protocol::LoginServerCharacterList response;

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 11> bind(*connection, L"{CALL dbo.spGetLoginCharacters(?)}");

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
    int16_t hp;
    bind.BindCol(5, hp);
    int16_t mp;
    bind.BindCol(6, mp);
    int16_t str;
    bind.BindCol(7, str);
    int16_t dex;
    bind.BindCol(8, dex);
    int16_t luk;
    bind.BindCol(9, luk);
    int16_t int_;
    bind.BindCol(10, int_);

    if (bind.Execute()) {
      do {
        int16_t count = 0;
        bind.GetResultColumnCount(&count);
        if (count <= 0) continue;

        while (bind.Fetch()) {
          std::optional<std::string> name_str = ConvertToUtf8(name);
          if (!name_str.has_value()) continue;

          protocol::LoginCharacter* character = response.add_characters();
          character->set_id(id);
          character->set_name(name_str.value());
          character->set_level(level);
          character->set_type(type);
          character->set_job(job);
          character->set_hp(hp);
          character->set_mp(mp);
          character->set_str(str);
          character->set_dex(dex);
          character->set_luk(luk);
          character->set_int_(int_);
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  login_session->Send(LoginClientPacketHandler::MakeSendBuffer(response));
}
