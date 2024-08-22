#pragma once
#include "network/protocol/game_enum.pb.h"

namespace game {
  struct KeyMapEntry {
    protocol::KeyType type = protocol::KEY_TYPE_UNSPECIFIED;
    int32_t data = 0;

    KeyMapEntry() = default;
    KeyMapEntry(const protocol::KeyType key_type, const int32_t key_data) : type(key_type), data(key_data) {}
  };

  class KeyMap {
  public:
    void SetKey(protocol::KeyCode key, protocol::KeyType type);
    void SetItem(protocol::KeyCode key, int32_t item_id);
    void SetSkill(protocol::KeyCode key, int32_t skill_id);

    std::unordered_map<protocol::KeyCode, std::shared_ptr<KeyMapEntry>> GetAll() const;

  public:
    /*PlayerDbQueue 에서 호출*/
    bool TryLoadFromDb(int32_t player_id);
    bool TrySaveToDb(int32_t player_id);

  private:
    std::optional<std::wstring> SerializeToXml() const;

  private:
    std::unordered_map<protocol::KeyCode, std::shared_ptr<KeyMapEntry>> _key_map;
    bool _is_dirty = false;
  };
}
