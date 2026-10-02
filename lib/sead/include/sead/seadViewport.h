#pragma once

#include "decomp.h"
#include "sead/seadBoundBox2.h"

namespace sead {
// RTTI N4sead8ViewportE @ 0x008D2258
// vtable 0x00906EF8 (vptr 0x00906F00), offset_to_top 0, 2 entries
class Viewport : public ::sead::BoundBox2<float>
{
public:
    Viewport(); // ctor candidate(s) 0x00561DF8, 0x00561E4C (unverified)
    virtual void vf_0x00(); // 0x00561F6C slot 0x00 | virtual slot, introduced by sead::Viewport
    virtual void vf_0x04(); // 0x00561F68 slot 0x04 | virtual slot, introduced by sead::Viewport
};
} // namespace sead
