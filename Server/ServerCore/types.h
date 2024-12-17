#pragma once

#include <atomic>
#include <mutex>

#include "memory/allocator.h"

// ReSharper disable CppInconsistentNaming

template <typename T>
using Atomic = std::atomic<T>;
using Mutex = std::mutex;
using CondVar = std::condition_variable;
using UniqueLock = std::unique_lock<Mutex>;
using LockGuard = std::lock_guard<Mutex>;

using String = std::wstring;
using StringView = std::wstring_view;

#define DECLARE_SHARED_PTR(name) using name##Ref = std::shared_ptr<class name>;
#define DECLARE_CONST_SHARED_PTR(name) using Const##name##Ref = std::shared_ptr<const class name>;

DECLARE_SHARED_PTR(IocpCore);
DECLARE_SHARED_PTR(IocpObject);
DECLARE_SHARED_PTR(Session);
DECLARE_SHARED_PTR(PacketSession);
DECLARE_SHARED_PTR(Listener);
DECLARE_SHARED_PTR(ServerService);
DECLARE_SHARED_PTR(ClientService);
DECLARE_SHARED_PTR(SendBuffer);
DECLARE_SHARED_PTR(SendBufferChunk);
DECLARE_SHARED_PTR(Job);
DECLARE_SHARED_PTR(JobQueue);

DECLARE_CONST_SHARED_PTR(Session);

#define size16(val) static_cast<int16_t>(sizeof(val))
#define size32(val) static_cast<int32_t>(sizeof(val))
#define length16(arr) static_cast<int16_t>(sizeof(arr) / sizeof((arr)[0]))
#define length32(arr) static_cast<int32_t>(sizeof(arr) / sizeof((arr)[0]))

// #define _STOMP
