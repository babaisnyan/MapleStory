#pragma once

namespace game {
  class Player;

  class GamePacketCreator {
  public:
    static SendBufferRef GetClientEnterFailResponse();
    static SendBufferRef GetClientEnterSuccessResponse(const std::shared_ptr<Player>& player);
  };

}
