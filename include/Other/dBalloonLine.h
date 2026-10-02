#pragma once

#include "decomp.h"
#include "Bs/dBsGrRenderer.h"
#include "Bs/dBsGrRenderer_Functor.h"

// RTTI 11BalloonLine @ 0x008CB234
// vtable 0x008EC7A4 (vptr 0x008EC7AC), offset_to_top 0, 3 entries
class BalloonLine : public ::BsGrRenderer::Functor
{
public:
    BalloonLine(); // ctor address unknown
    virtual void vf_0x00(); // 0x001C23DC slot 0x00 | virtual slot, introduced by BalloonLine
    virtual void vf_0x04(); // 0x001C23A4 slot 0x04 | virtual slot, introduced by BalloonLine
    virtual void vf_0x08(); // 0x001C1E18 slot 0x08 | virtual slot, introduced by BalloonLine
};
