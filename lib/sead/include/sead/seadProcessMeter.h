#pragma once

#include "decomp.h"

namespace sead {
class Heap;

// The process meter (only its singleton is in this build: createInstance allocates 16 bytes for the
// SingletonDisposer_ and nothing else). Names are ours.
class ProcessMeter
{
public:
    class SingletonDisposer_;

    static ProcessMeter* createInstance(Heap* heap); // 0x0054103C

    static ProcessMeter* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4]; // 0x00, the SingletonDisposer_
};
ASSERT_SIZE(ProcessMeter, 0x10);
} // namespace sead
