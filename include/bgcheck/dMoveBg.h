#pragma once

#include "decomp.h"
#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"

namespace bgcheck {
// RTTI N7bgcheck6MoveBgE @ 0x008D3EF4
// vtable 0x0090BC94 (vptr 0x0090BC9C), offset_to_top 0, 4 entries
class MoveBg : public ::collision::World::Body
{
public:
    MoveBg(); // ctor candidate(s) 0x0061532C (unverified)
    virtual void vf_0x00(); // 0x00615384 slot 0x00 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x04(); // 0x00615360 slot 0x04 | virtual slot, introduced by collision::World::Body
    virtual void vf_0x08(); // 0x00615270 slot 0x08 | virtual slot, introduced by bgcheck::MoveBg
    virtual void vf_0x0C(); // 0x0061526C slot 0x0C | virtual slot, introduced by bgcheck::MoveBg
};
} // namespace bgcheck
