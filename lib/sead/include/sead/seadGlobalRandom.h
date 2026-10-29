#pragma once

#include "decomp.h"
#include "sead/seadRandom.h"

namespace sead {
class Heap;

// The random number generator of the program, seeded with the tick count. A singleton whose
// SingletonDisposer_ lies behind the generator (at 0x10). Names are ours.
class GlobalRandom : public Random
{
public:
    class SingletonDisposer_;

    GlobalRandom() { init(); }

    static GlobalRandom* createInstance(Heap* heap); // 0x00132E64
    static void deleteInstance(); // 0x0012C504

    static GlobalRandom* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 mSingletonDisposerBuffer[4]; // 0x10, the SingletonDisposer_
};
ASSERT_SIZE(GlobalRandom, 0x20);
} // namespace sead
