#pragma once

#include "decomp.h"

class TourResult
{
public:
    class SingletonDisposer_;
    void ChangeScore(netgame::PlayerNo, long, SvFgName const&); // 0x001B82A4 | libgarden [tier A]

    // (names are ours; only what the stage code reads)
    static TourResult* s_pInstance;

    u8 _unknown0[0x1C];
    s8 m_Tour;              // 0x01C the tour (see GetTourType)
    u8 _unknown1D[0xEF];
    u8 m_State;             // 0x10C
};
