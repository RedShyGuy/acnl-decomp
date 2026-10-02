#pragma once

#include "decomp.h"
#include "Bs/dBsLightFixBase.h"

// RTTI 15BsLightFixParam @ 0x008CBEF0
// vtable 0x008F1284 (vptr 0x008F128C), offset_to_top 0, 21 entries
class BsLightFixParam : public ::BsLightFixBase
{
public:
    BsLightFixParam(); // ctor candidate(s) 0x007EF610 (unverified)
    virtual ~BsLightFixParam(); // 0x00285A40 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00285A1C slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00285740 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void vf_0x48(); // 0x00285A0C slot 0x48 | virtual slot, introduced by BsLightBase
};
