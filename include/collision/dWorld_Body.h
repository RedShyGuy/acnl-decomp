#pragma once

#include "decomp.h"
#include "collision/dWorld.h"

namespace collision {
class Shape;
} // namespace collision

// a body of the collision world: a shape, the bits of its groups and the bits of the groups it touches (two bodies
// touch if each one's group is in the other's mask; names are ours)
// RTTI N9collision5World4BodyE @ 0x008D4104
// vtable 0x0090C28C (vptr 0x0090C294), offset_to_top 0, 2 entries
class collision::World::Body
{
public:
    Body(); // 0x006F89B4
    Body(Shape* shape, u32 group, u32 mask); // 0x006F899C
    virtual ~Body(); // 0x006F89DC slot 0x00
    // 0x006F89D4 (deleting dtor)

    void Set(Shape* shape, u32 group, u32 mask); // 0x006F8990
    // (armlink removed a call at the start)
    void SetMask(u32 mask); // 0x006F8954
    // (inline; the copy lies in the shared code and starts with a removed call)
    DECOMP_EMIT_COPY DECOMP_NOINLINE u32 GetMask() const { return m_Mask; } // 0x00771C20

    Shape* m_pShape; // 0x4
    u32 m_Group;     // 0x8
    u32 m_Mask;      // 0xC
};
ASSERT_SIZE(collision::World::Body, 0x10);
