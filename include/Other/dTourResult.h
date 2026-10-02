#pragma once

#include "decomp.h"

class TourResult
{
public:
    class SingletonDisposer_;
    void ChangeScore(netgame::PlayerNo, long, SvFgName const&); // 0x001B82A4 | libgarden [tier A]
};
