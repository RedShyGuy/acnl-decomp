#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"

namespace nn {
namespace nex {
class ObjectThreadRoot;

// Every millisecond one of its threads (at random) gets a high or a low priority (a test of the
// scheduling of the threads). armlink kept only its thread function (through the shared step table
// 0x8998A8; no constructor, vtable or RTTI); the class name, the base part and the member names are
// ours.
class ThreadPriorityShuffler
{
public:
    // the priorities that the threads get
    static const s32 PRIORITY_HIGH = 16;
    static const s32 PRIORITY_LOW = 24;
    static const u32 MAX_THREAD_COUNT = 256;

    void Run(int unused); // 0x0037E4BC

    u8 m_Unknown0x00[0x38];                          // 0x00 (unknown base)
    u32 m_ThreadCount;                               // 0x38
    ObjectThreadRoot* m_pThreads[MAX_THREAD_COUNT];  // 0x3C
    CriticalSection m_Lock;                          // 0x43C (of the threads)
    bool m_IsStopped;                                // 0x464
};
ASSERT_OFFSET(ThreadPriorityShuffler, m_Lock, 0x43C);
ASSERT_OFFSET(ThreadPriorityShuffler, m_IsStopped, 0x464);
} // namespace nex
} // namespace nn
