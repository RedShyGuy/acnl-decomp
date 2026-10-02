#pragma once

#include "decomp.h"
#include "collision/dShape.h"

namespace collision {
// RTTI N9collision8CylinderE @ 0x008D4124
// vtable 0x0090C2C4 (vptr 0x0090C2CC), offset_to_top 0, 3 entries
class Cylinder : public ::collision::Shape
{
public:
    Cylinder(); // ctor candidate(s) 0x006F8F1C, 0x006F8F48 (unverified)
    virtual void vf_0x00(); // 0x006F8F94 slot 0x00 | virtual slot, introduced by collision::Cylinder
    virtual void vf_0x04(); // 0x006F8F70 slot 0x04 | virtual slot, introduced by collision::Cylinder
    virtual void vf_0x08(); // 0x006F8EB8 slot 0x08 | virtual slot, introduced by collision::Cylinder
};
} // namespace collision
