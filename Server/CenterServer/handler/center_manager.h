#pragma once
#include "center_handler.h"

#include "memory/memory.h"

class CenterManager {
private:
  CenterManager() : _center_handler(MakeShared<CenterHandler>()) {}
  ~CenterManager() = default;

public:
  static CenterManager& GetInstance() {
    static CenterManager instance;
    return instance;
  }

  void HandleServerRegisterRequest(const PacketSessionRef& session, const protocol::CenterClientRegisterRequest& request);

private:
  std::shared_ptr<CenterHandler> _center_handler;
};
