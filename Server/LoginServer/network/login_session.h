#pragma once
#include "network/session.h"

#include "protocol/login_struct.pb.h"

class LoginSession : public PacketSession {
public:
  ~LoginSession() override {
    std::cout << "LoginSession Destructor\n";
  }

public:
  int32_t GetAccountId() const { return _account_id; }
  void    SetAccountId(const int32_t account_id) { _account_id = account_id; }

  Vector<protocol::LoginCharacter>& GetCharacterList() { return _character_list; }
  void AddCharacter(const protocol::LoginCharacter& character) { _character_list.push_back(character); }

protected:
  void OnConnected() override;
  void OnDisconnected() override;
  void OnRecvPacket(std::byte* buffer, const int32_t len) override;
  void OnSend(const int32_t len) override;

private:
  int32_t _account_id = 0;
  Vector<protocol::LoginCharacter> _character_list;
};
