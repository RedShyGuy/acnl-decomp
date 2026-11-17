#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"

namespace collision {
// A shape of the collision world (no vtable of its own in the binary: only built as a base; names are ours).
// RTTI N9collision5ShapeE @ 0x008D40FC
class Shape
{
public:
    // m_Kind (after the constructors of the shapes); the index into the table of the tests (dCollide.cpp)
    enum Kind : u8
    {
        KIND_SPHERE = 0,
        KIND_AABB = 1,
        KIND_CYLINDER = 2,
        KIND_CAPSULE = 3,
        KIND_TRIANGLE = 4,
        KIND_NUM = 5,
    };

    // (inline)
    explicit Shape(Kind kind) : m_Kind(kind) {}
    virtual ~Shape() {}
    // moves the shape by offset
    virtual void Translate(const nn::math::VEC3& offset) = 0; // slot 0x08

    Kind m_Kind; // 0x4
};

// a point of contact between two shapes (names are ours): the normal points from the second shape to the first
struct Contact
{
    // (empty; the arrays of contacts are built with __aeabi_vec_ctor)
    Contact(); // 0x006F8EB4
    Contact& operator=(const Contact& other); // 0x006F8E90

    nn::math::VEC3 point;  // 0x00
    nn::math::VEC3 normal; // 0x0C
    f32 depth;             // 0x18 how far the shapes overlap
};
ASSERT_SIZE(Contact, 0x1C);

// the contacts of two shapes into out (size bytes): their number (0: none; a test that needs more room than size
// gives 0; the pairs without a test give 0)
s32 Collide(Contact* out, u32 size, const Shape* a, const Shape* b); // 0x006F8DA4
} // namespace collision
