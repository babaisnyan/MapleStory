#pragma once

#include "center_handler.h"

namespace center {
  class CenterManager {
  private:
    CenterManager() : _center_handler(std::make_shared<CenterHandler>()) {}
    ~CenterManager() = default;

  public:
    static CenterManager& GetInstance() {
      static CenterManager instance;
      return instance;
    }

    void HandleServerRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& request);
    void HandleServerMigrationRequest(const PacketSessionRef& session, const protocol::CenterClientMigrationRequest& request);
    void HandleServerMigrationResponse(const PacketSessionRef& session, const protocol::CenterClientMigrationResponse& response);

  private:
    std::shared_ptr<CenterHandler> _center_handler;
  };
}
