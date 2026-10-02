#pragma once

#include "decomp.h"
#include "Resource/dResourceGetSklVis.h"

// RTTI 20ResourceGetSklVisMat @ 0x008CCBF4
// vtable 0x008F59E8 (vptr 0x008F59F0), offset_to_top 0, 9 entries
// vtable 0x008F5A3C (vptr 0x008F5A44), offset_to_top -104, 11 entries
class ResourceGetSklVisMat : public ::ResourceGetSklVis
{
public:
    ResourceGetSklVisMat(); // ctor address unknown
    virtual void vf_0x00(); // 0x00321D18 slot 0x00 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x04(); // 0x00321CA4 slot 0x04 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x08(); // 0x003215E4 slot 0x08 | virtual slot, introduced by ResourceGetSkeletal
    virtual void vf_0x0C(); // 0x00321868 slot 0x0C | virtual slot, introduced by ResourceGetSkeletal
};
