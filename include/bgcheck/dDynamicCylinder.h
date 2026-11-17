#pragma once

#include "decomp.h"
#include "collision/dCylinder.h"
#include "collision/dWorld.h"
#include "collision/dWorld_Body.h"
#include "nn/math/math_Vector3.h"

namespace bgcheck {
// a body with a cylinder that can move (names are ours)
// RTTI N7bgcheck15DynamicCylinderE @ 0x008D3EE8
// vtable 0x0090BC84 (vptr 0x0090BC8C), offset_to_top 0, 2 entries
class DynamicCylinder : public ::collision::World::Body
{
public:
    DynamicCylinder(); // 0x00615040
    virtual ~DynamicCylinder(); // 0x00615084 slot 0x00
    // 0x00615060 (deleting dtor)

    // the cylinder and the values of the body (the copy lies in the code of the collision world)
    void Set(const nn::math::VEC3* center, f32 radius, f32 height, u32 group, u32 mask); // 0x006F8960
    // the cylinder (the copy lies in the code of the collision world and falls into Cylinder::Set)
    void SetCylinder(const nn::math::VEC3* center, f32 radius, f32 height); // 0x006F8EF0

    collision::Cylinder m_Cylinder; // 0x10
};
ASSERT_SIZE(DynamicCylinder, 0x2C);

// the body joins / leaves the collision world 0x009880A8 (NULL: nothing)
void AddBody(collision::World::Body* body); // 0x00615208
void RemoveBody(collision::World::Body* body); // 0x00615244
} // namespace bgcheck
