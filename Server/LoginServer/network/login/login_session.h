#pragma once
#include "data/login_character.h"

#include "network/session.h"
#include "network/protocol/login_struct.pb.h"

class LoginSession final : public PacketSession {
public:
  ~LoginSession() override {
    std::cout << "LoginSession Destructor\n";
  }

public:
  int32_t GetAccountId() const { return _account_id; }
  void SetAccountId(const int32_t account_id) { _account_id = account_id; }

  Vector<std::shared_ptr<LoginCharacter>>& GetCharacterList() { return _character_list; }
  int32_t GetCharacterCount() const { return static_cast<int32_t>(_character_list.size()); }

  void AddCharacter(const std::shared_ptr<LoginCharacter>& character) { _character_list.push_back(character); }
  void RemoveCharacter(const std::shared_ptr<LoginCharacter>& character) { std::erase(_character_list, character); }
  void ClearCharacters() { _character_list.clear(); }

protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:
  int32_t _account_id = 0;
  Vector<std::shared_ptr<LoginCharacter>> _character_list;
};

using LoginSessionRef = std::shared_ptr<LoginSession>;