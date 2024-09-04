#include "pch.h"
#include "key_map.h"

#include "database/db_bind.h"
#include "database/db_connection_pool.h"

#include <pugixml.hpp>

void KeyMap::SetKey(const protocol::KeyCode key, const protocol::KeyType type) {
  if (key == protocol::KEY_CODE_UNSPECIFIED) {
    return;
  }

  if (!_key_map.contains(key)) {
    _key_map[key] = std::make_shared<KeyMapEntry>();
  }

  _key_map[key]->type = type;
  _is_dirty = true;
}

void KeyMap::SetItem(const protocol::KeyCode key, const int32_t item_id) {
  if (key == protocol::KEY_CODE_UNSPECIFIED) {
    return;
  }

  if (!_key_map.contains(key)) {
    _key_map[key] = std::make_shared<KeyMapEntry>();
  }

  _key_map[key]->type = protocol::KEY_TYPE_ITEM;
  _key_map[key]->data = item_id;
  _is_dirty = true;
}

void KeyMap::SetSkill(const protocol::KeyCode key, const int32_t skill_id) {
  if (key == protocol::KEY_CODE_UNSPECIFIED) {
    return;
  }

  if (!_key_map.contains(key)) {
    _key_map[key] = std::make_shared<KeyMapEntry>();
  }

  _key_map[key]->type = protocol::KEY_TYPE_SKILL;
  _key_map[key]->data = skill_id;
  _is_dirty = true;
}

std::unordered_map<protocol::KeyCode, std::shared_ptr<KeyMapEntry>> KeyMap::GetAll() const {
  return _key_map;
}

bool KeyMap::TryLoadFromDb(int32_t player_id) {
  _key_map.clear();
  _is_dirty = false;
  bool success = true;

  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<1, 3> bind(*connection, L"{CALL dbo.spLoadKeymap(?)}");
    bind.BindParam(0, player_id);

    int32_t key_code = 0;
    bind.BindCol(0, key_code);
    int32_t key_type = 0;
    bind.BindCol(1, key_type);
    int32_t data = 0;
    bind.BindCol(2, data);

    if (bind.Execute()) {
      do {
        int16_t count;
        bind.GetResultColumnCount(&count);

        if (count != 3) {
          success = false;
          break;
        }

        while (bind.Fetch()) {
          if (key_code == 0) {
            continue;
          }

          const auto key = static_cast<protocol::KeyCode>(key_code);
          const auto type = static_cast<protocol::KeyType>(key_type);
          const auto entry = std::make_shared<KeyMapEntry>(type, data);
          _key_map.emplace(key, entry);
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  return success;
}

bool KeyMap::TrySaveToDb(int32_t player_id) {
  if (!_is_dirty) {
    return true;
  }

  const auto xml = SerializeToXml();

  if (!xml.has_value()) {
    _is_dirty = false;
    return false;
  }

  bool success = false;

  if (const auto connection = DbConnectionPool::GetInstance().GetConnection()) {
    DbBind<2, 1> bind(*connection, L"{CALL dbo.spSaveKeymapXml(?, ?)}");

    bind.BindParam(0, player_id);
    bind.BindParam(1, xml.value());

    int temp = 0;
    bind.BindCol(0, temp);

    if (bind.Execute()) {
      do {
        int16_t count;
        bind.GetResultColumnCount(&count);

        if (count == 0) {
          break;
        }

        while (bind.Fetch()) {
          success = true;
        }
      } while (bind.GetMoreResult() != SQL_NO_DATA);
    }

    DbConnectionPool::GetInstance().ReleaseConnection(connection);
  }

  _is_dirty = !success;
  return success;
}

std::optional<std::wstring> KeyMap::SerializeToXml() const {
  if (_key_map.empty()) {
    return std::nullopt;
  }

  pugi::xml_document doc;
  auto root = doc.append_child("ms");
  auto children = root.append_child("keymaps");

  for (const auto& [key, entry] : _key_map) {
    auto child = children.append_child("keymap");
    child.append_attribute("keyCode").set_value(key);
    child.append_attribute("keyType") = entry->type;
    child.append_attribute("data") = entry->data;
  }

  std::wstringstream stream;
  doc.save(stream);
  return stream.str();
}
