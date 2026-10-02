#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision6SphereE @ 0x008D410C
// vtable 0x0090C29C (vptr 0x0090C2A4), offset_to_top 0, 3 entries
class Sphere : public ::collision::Shape
{
public:
    Sphere(); // ctor candidate(s) 0x006F8C84, 0x006F8CAC (unverified)
    virtual void vf_0x00(); // 0x006F8CD4 slot 0x00 | virtual slot, introduced by collision::Sphere
    virtual void vf_0x04(); // 0x006F8CD0 slot 0x04 | virtual slot, introduced by collision::Sphere
    virtual void vf_0x08(); // 0x006F8C2C slot 0x08 | virtual slot, introduced by collision::Sphere
};
} // namespace collision
