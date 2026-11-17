#include "collision/dAABB.h"

namespace collision {
// 0x006F8910 (name is ours)
collision::AABB::AABB() : Shape(KIND_AABB)
{
}

// 0x006F892C slot 0x00
// 0x006F8928 (deleting dtor)
collision::AABB::~AABB()
{
    // empty (in the original too)
}

// 0x006F87AC slot 0x08 (name is ours)
void collision::AABB::Translate(const nn::math::VEC3& offset)
{
    m_Max.x += offset.x;
    m_Max.y += offset.y;
    m_Max.z += offset.z;
    m_Min.x += offset.x;
    m_Min.y += offset.y;
    m_Min.z += offset.z;
}

// 0x006F88DC (name is ours)
void collision::AABB::Set(const nn::math::VEC3& min, const nn::math::VEC3& max)
{
    m_Min = min;
    m_Max = max;
}

// 0x006F8884 (name is ours)
void collision::AABB::SetFromBase(const nn::math::VEC3& base, f32 width, f32 depth, f32 height)
{
    const f32 halfWidth = width * 0.5f;
    const f32 halfDepth = depth * 0.5f;
    m_Min.x = base.x - halfWidth;
    m_Min.y = base.y;
    m_Min.z = base.z - halfDepth;
    m_Max.x = base.x + halfWidth;
    m_Max.y = base.y + height;
    m_Max.z = base.z + halfDepth;
}
} // namespace collision
