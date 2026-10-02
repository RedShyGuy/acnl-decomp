#pragma once

#include "decomp.h"
#include "Bs/dBsGrRenderer.h"
#include "Bs/dBsGrRenderer_Functor.h"

// RTTI 12WaterCurtain @ 0x008CB844
// vtable 0x008EE91C (vptr 0x008EE924), offset_to_top 0, 3 entries
class WaterCurtain : public ::BsGrRenderer::Functor
{
public:
    WaterCurtain(); // ctor candidate(s) 0x0020E694 (unverified)
    virtual void vf_0x00(); // 0x0020E6F8 slot 0x00 | virtual slot, introduced by WaterCurtain
    virtual void vf_0x04(); // 0x0020E6CC slot 0x04 | virtual slot, introduced by WaterCurtain
    virtual void vf_0x08(); // 0x0020E178 slot 0x08 | virtual slot, introduced by WaterCurtain
};
