#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead15GraphicsContextE @ 0x008D1894
// vtable 0x00905AEC (vptr 0x00905AF4), offset_to_top 0, 2 entries
class GraphicsContext
{
public:
    GraphicsContext(); // ctor candidate(s) 0x0011EC6C, 0x0012091C (unverified)
    virtual void vf_0x00(); // 0x00545D38 slot 0x00 | virtual slot, introduced by sead::GraphicsContext
    virtual void vf_0x04(); // 0x00545D28 slot 0x04 | virtual slot, introduced by sead::GraphicsContext
    void apply() const; // 0x00121664 | nintendogs:bytes [tier A]
};
} // namespace sead
