#pragma once

#include "decomp.h"
#include "Bs/dBsLightPointBase.h"

// RTTI 15BsLightPointFix @ 0x008CBEFC
// vtable 0x008F12E0 (vptr 0x008F12E8), offset_to_top 0, 20 entries
class BsLightPointFix : public ::BsLightPointBase
{
public:
    BsLightPointFix(); // ctor candidate(s) 0x007EF638 (unverified)
    virtual ~BsLightPointFix(); // 0x00285BDC slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00285BB0 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00285A60 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void vf_0x48(); // 0x00285BA0 slot 0x48 | virtual slot, introduced by BsLightBase
};
