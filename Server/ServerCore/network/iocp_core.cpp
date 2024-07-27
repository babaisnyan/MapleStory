#include "pch.h"
#include "iocp_core.h"
#include "iocp_event.h"

IocpCore::IocpCore() {
  _iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
                                        nullptr, 0, 0);

  ASSERT_CRASH(_iocp_handle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore() {
  CloseHandle(_iocp_handle);
}

bool IocpCore::Register(const IocpObjectRef& iocp_object) const {
  const HANDLE handle = CreateIoCompletionPort(iocp_object->GetHandle(),
                                               _iocp_handle,
                                               0,
                                               0);

  return handle != INVALID_HANDLE_VALUE;
}

bool IocpCore::Dispatch(const uint32_t timeout_ms) const {
  DWORD num_of_bytes = 0;
  ULONG_PTR key = 0;
  IocpEvent* iocp_event = nullptr;

  const bool result = GetQueuedCompletionStatus(_iocp_handle,
                                                &num_of_bytes,
                                                &key,
                                                reinterpret_cast<OVERLAPPED**>(&iocp_event),
                                                timeout_ms);

  if (result) {
    const IocpObjectRef iocp_object = iocp_event->GetOwner();

    iocp_object->Dispatch(iocp_event, num_of_bytes);
  } else {
    const int32_t error_code = WSAGetLastError();

    if (error_code == WAIT_TIMEOUT) {
      return false;
    }

    const IocpObjectRef iocp_object = iocp_event->GetOwner();

    iocp_object->Dispatch(iocp_event, num_of_bytes);
  }

  return true;
}
