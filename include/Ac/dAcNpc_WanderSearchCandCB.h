#pragma once

#include "decomp.h"
#include "Ac/dAcNpc.h"
#include "Road/dRoadSearchSetupCandCB.h"

// RTTI N5AcNpc18WanderSearchCandCBE @ 0x008D2558
// vtable 0x00907408 (vptr 0x00907410), offset_to_top 0, 1 entries
class AcNpc::WanderSearchCandCB : public ::RoadSearchSetupCandCB
{
public:
    WanderSearchCandCB(); // ctor address unknown
    virtual void vf_0x00(); // 0x00579C60 slot 0x00 | virtual slot, introduced by AcNpc::WanderSearchCandCB
};
