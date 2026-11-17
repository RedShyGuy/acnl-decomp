#include "collision/dCapsule.h"

namespace collision {
// 0x006F8D78 (name is ours)
collision::Capsule::Capsule() : Shape(KIND_CAPSULE), m_Radius(0.0f)
{
}

// 0x006F8DA0 slot 0x00
// 0x006F8D9C (deleting dtor)
collision::Capsule::~Capsule()
{
    // empty (in the original too)
}

// 0x006F8CD8 slot 0x08 (name is ours)
void collision::Capsule::Translate(const nn::math::VEC3& offset)
{
    m_Start.x += offset.x;
    m_Start.y += offset.y;
    m_Start.z += offset.z;
    m_End.x += offset.x;
    m_End.y += offset.y;
    m_End.z += offset.z;
}

// 0x006F8D40 (name is ours)
void collision::Capsule::Set(const nn::math::VEC3& start, const nn::math::VEC3& end, f32 radius)
{
    m_Start = start;
    m_End = end;
    m_Radius = radius;
}
} // namespace collision
