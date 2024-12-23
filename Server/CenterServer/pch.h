#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "core_pch.h"

#include "network/protocol/center_enum.pb.h"

#include "utils/constants.h"

namespace center {
  using CenterSessionRef = std::shared_ptr<class CenterSession>;
}

using namespace center;
