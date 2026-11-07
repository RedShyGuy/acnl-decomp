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
    // whether the input of the game is ignored (the menus and the network steps wait; meaning of the two flags unknown)
    static bool IsInputBlocked(); // 0x005CF1AC (name is ours)

    u8 _unknown0[0x10];
    u8 m_Unknown0x10;       // 0x010
    u8 _unknown11[0xFAC - 0x11];
    u8 m_UnknownFAC;        // 0xFAC
    bool m_UnknownFAD;      // 0xFAD (meaning unknown)
    u8 _unknownFAE[2];
    bool m_FB0;             // 0xFB0 (meaning unknown)
};
} // namespace applet
