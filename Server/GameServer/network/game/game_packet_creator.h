#pragma once

namespace game {
  class GamePacketCreator {
  public:
    static SendBufferRef GetClientEnterResponse(bool success);

  };

}
