#pragma once

#include "decomp.h"

namespace photo {
class Mgr
{
public:
    class SingletonDisposer_;

    // frees the buffers of the photos in the photo heap (for the font font::Mgr loads there; name is ours)
    void FreeBuffers(); // 0x005B3848
    // after the font is gone again
    void Func5B321C(); // 0x005B321C

    static Mgr* s_pInstance;
};
} // namespace photo
