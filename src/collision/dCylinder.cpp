#include "collision/dCylinder.h"

namespace collision {
// 0x006F8F48 (name is ours)
collision::Cylinder::Cylinder() : Shape(KIND_CYLINDER), m_Radius(0.0f), m_Height(0.0f)
{
}

// 0x006F8F1C (name is ours)
collision::Cylinder::Cylinder(const nn::math::VEC3& center, f32 radius, f32 height)
    : Shape(KIND_CYLINDER), m_Center(center), m_Radius(radius), m_Height(height)
{
}

// 0x006F8F94 slot 0x00
// 0x006F8F70 (deleting dtor)
collision::Cylinder::~Cylinder()
{
    // empty (in the original too)
}

// 0x006F8EB8 slot 0x08 (name is ours)
void collision::Cylinder::Translate(const nn::math::VEC3& offset)
{
    m_Center.x += offset.x;
    m_Center.y += offset.y;
    m_Center.z += offset.z;
}

// 0x006F8EF8 (name is ours)
void collision::Cylinder::Set(const nn::math::VEC3* center, f32 radius, f32 height)
{
    m_Center = *center;
    m_Radius = radius;
    m_Height = height;
}

} // namespace collision
