#include "pch.h"
#include "core_tls.h"


thread_local uint32_t            LThreadId;
thread_local uint64_t            LEndTickCount;
thread_local std::stack<int32_t> LLockStack;
thread_local SendBufferChunkRef  LSendBufferChunk;
thread_local JobQueue*           LJobQueue;
