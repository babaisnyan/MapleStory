#pragma once

namespace center {
  struct MigrationData {
    MigrationData(const int32_t character_id, const std::weak_ptr<CenterSession>& from_session, const std::weak_ptr<CenterSession>& to_session)
      : character_id(character_id), from_session(from_session), to_session(to_session) {}

    int32_t character_id;
    std::weak_ptr<CenterSession> from_session;
    std::weak_ptr<CenterSession> to_session;
  };
}
