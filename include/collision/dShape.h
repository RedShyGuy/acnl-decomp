#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace collision {
// A shape of the collision world (no vtable of its own in the binary: only built as a base; names are ours).
// RTTI N9collision5ShapeE @ 0x008D40FC
class Shape
{
public:
    // m_Kind (after the constructors of the shapes)
    enum Kind : u8
    {
        KIND_SPHERE = 0,
        KIND_AABB = 1,
        KIND_CYLINDER = 2,
        KIND_CAPSULE = 3,
        KIND_TRIANGLE = 4,
    };

    // (inline)
    explicit Shape(Kind kind) : m_Kind(kind) {}
    virtual ~Shape() {}
    // moves the shape by offset
    virtual void Translate(const nn::math::VEC3& offset) = 0; // slot 0x08

    Kind m_Kind; // 0x4
};
} // namespace collision
