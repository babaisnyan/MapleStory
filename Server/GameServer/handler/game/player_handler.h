#pragma once
#include "network/protocol/game_protocol.pb.h"

namespace game {
  class PlayerHandler {
  public:
    static void HandleKeySettingChange(const GameSessionRef& session, const protocol::GameClientChangeKeySetting& packet);
  };
}
