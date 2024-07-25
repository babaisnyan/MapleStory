#pragma once

#include "center_handler.h"

namespace game {
  class CenterManager {
  private:
    CenterManager() : _center_handler(std::make_shared<CenterHandler>()) {}
    ~CenterManager() = default;

  public:
    static CenterManager& GetInstance() {
      static CenterManager instance;
      return instance;
    }

  public:
    void HandleMigrationResponse(const PacketSessionRef& session, const protocol::CenterServerMigrationResponse& packet);
    void HandleMigrationRequest(const PacketSessionRef& session, const protocol::CenterServerMigrationRequest& packet);

  private:
    std::shared_ptr<CenterHandler> _center_handler;
  };
}
