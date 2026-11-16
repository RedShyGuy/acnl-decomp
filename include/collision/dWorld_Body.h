#pragma once

#include "decomp.h"
#include "collision/dWorld.h"

namespace collision {
class Shape;
} // namespace collision

// a body of the collision world: a shape with two values (names are ours)
// RTTI N9collision5World4BodyE @ 0x008D4104
// vtable 0x0090C28C (vptr 0x0090C294), offset_to_top 0, 2 entries
class collision::World::Body
{
public:
    Body(); // 0x006F89B4
    Body(Shape* shape, u32 unk8, u32 unkC); // 0x006F899C
    virtual ~Body(); // 0x006F89DC slot 0x00
    // 0x006F89D4 (deleting dtor)

    Shape* m_pShape; // 0x4
    u32 m_Unk8;      // 0x8
    u32 m_UnkC;      // 0xC
};
ASSERT_SIZE(collision::World::Body, 0x10);
