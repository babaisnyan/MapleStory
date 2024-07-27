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

#include <nlohmann/json.hpp>

namespace game {
  using CenterSessionRef = std::shared_ptr<class CenterServerSession>;
}

using namespace game;
using namespace nlohmann;
