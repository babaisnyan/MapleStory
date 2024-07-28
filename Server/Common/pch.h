#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN 

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#endif

#include "core_pch.h"

#include <nlohmann/json.hpp>
using namespace nlohmann;

#endif 
