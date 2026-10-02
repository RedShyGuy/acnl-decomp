#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision7CapsuleE @ 0x008D4118
// vtable 0x0090C2B0 (vptr 0x0090C2B8), offset_to_top 0, 3 entries
class Capsule : public ::collision::Shape
{
public:
    Capsule(); // ctor candidate(s) 0x006F8D78 (unverified)
    virtual void vf_0x00(); // 0x006F8DA0 slot 0x00 | virtual slot, introduced by collision::Capsule
    virtual void vf_0x04(); // 0x006F8D9C slot 0x04 | virtual slot, introduced by collision::Capsule
    virtual void vf_0x08(); // 0x006F8CD8 slot 0x08 | virtual slot, introduced by collision::Capsule
};
} // namespace collision
