#pragma once

#include "decomp.h"

class SoundMgr
{
public:
    class SingletonDisposer_;
    static void PlaySound(SeID, float); // 0x0058DE9C | libgarden [tier A]
};
