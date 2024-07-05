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

  if (auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<2, 2> bind(*connection, L"CALL dbo.spLogin(?, ?)");

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
            case protocol::Success: [[fallthrough]];
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
