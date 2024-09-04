#include "pch.h"
#include "center_session_manager.h"

#include "center_server_session.h"

void CenterSessionManager::Send(const SendBufferRef& send_buffer) const {
  if (_center_session) {
    _center_session->Send(send_buffer);
  }
}
