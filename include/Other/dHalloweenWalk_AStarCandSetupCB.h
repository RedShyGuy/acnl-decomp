#pragma once

#include "decomp.h"
#include "Other/dHalloweenWalk.h"
#include "Road/dRoadSearchSetupCandCB.h"

// RTTI N13HalloweenWalk16AStarCandSetupCBE @ 0x008CD974
// vtable 0x008FB3C4 (vptr 0x008FB3CC), offset_to_top 0, 1 entries
class HalloweenWalk::AStarCandSetupCB : public ::RoadSearchSetupCandCB
{
public:
    AStarCandSetupCB(); // ctor address unknown
    virtual void vf_0x00(); // 0x0022D2B4 slot 0x00 | virtual slot, introduced by HalloweenWalk::AStarCandSetupCB
};
