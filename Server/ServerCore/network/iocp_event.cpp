#include "pch.h"
#include "iocp_event.h"

IocpEvent::IocpEvent(const EventType type) : _event_type(type) { Init(); }

void IocpEvent::Init() {
  Internal = 0;
  InternalHigh = 0;
  Offset = 0;
  OffsetHigh = 0;
  hEvent = nullptr;
}
