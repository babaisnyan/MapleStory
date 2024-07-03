#pragma once

class IocpEvent;

class IocpObject : public std::enable_shared_from_this<IocpObject> {
public:
  virtual HANDLE GetHandle() const noexcept = 0;
  virtual void Dispatch(IocpEvent* event, int32_t num_of_bytes = 0) = 0;
};

class IocpCore {
public:
  IocpCore();
  ~IocpCore();

  HANDLE GetHandle() const noexcept {
    return _iocp_handle;
  }

  bool Register(const IocpObjectRef& iocp_object) const;
  bool Dispatch(uint32_t timeout_ms = INFINITE) const;

private:
  HANDLE _iocp_handle;
};