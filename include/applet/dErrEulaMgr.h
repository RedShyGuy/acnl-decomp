#pragma once

#include "decomp.h"

namespace applet {
class ErrEulaMgr
{
public:
    class SingletonDisposer_;

    // (names are ours; only what the stage code reads)
    static ErrEulaMgr* s_pInstance;

    u8 _unknown0[0xFB0];
    bool m_FB0;             // 0xFB0 (meaning unknown)
};
} // namespace applet
