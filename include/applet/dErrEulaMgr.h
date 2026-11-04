#pragma once

#include "decomp.h"

namespace applet {
class ErrEulaMgr
{
public:
    class SingletonDisposer_;

    // (names are ours; only what the stage code reads)
    static ErrEulaMgr* s_pInstance;

    // the flag at 0xFAD (meaning unknown; g3d::Draw draws nothing while it is set)
    static bool IsUnknownFADSet(); // 0x005CEE38 (name is ours)

    u8 _unknown0[0xFAD];
    bool m_UnknownFAD;      // 0xFAD (meaning unknown)
    u8 _unknownFAE[2];
    bool m_FB0;             // 0xFB0 (meaning unknown)
};
} // namespace applet
