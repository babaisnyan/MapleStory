#pragma once

namespace game {
  class Player;

  class GamePacketCreator {
  public:
    static SendBufferRef GetClientEnterFailResponse();
    static SendBufferRef GetClientEnterSuccessResponse(const std::shared_ptr<Player>& player);
    static SendBufferRef GetPlayerLeaveResponse(int32_t int32);
  };

}
