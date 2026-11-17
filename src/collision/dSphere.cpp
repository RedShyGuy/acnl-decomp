#include "collision/dSphere.h"

namespace collision {
// 0x006F8CAC (name is ours)
collision::Sphere::Sphere() : Shape(KIND_SPHERE), m_Radius(0.0f)
{
}

// 0x006F8C84 (name is ours)
collision::Sphere::Sphere(const nn::math::VEC3& center, f32 radius) : Shape(KIND_SPHERE), m_Center(center), m_Radius(radius)
{
}

// 0x006F8CD4 slot 0x00
// 0x006F8CD0 (deleting dtor)
collision::Sphere::~Sphere()
{
    // empty (in the original too)
}

// 0x006F8C2C slot 0x08 (name is ours)
void collision::Sphere::Translate(const nn::math::VEC3& offset)
{
    m_Center.x += offset.x;
    m_Center.y += offset.y;
    m_Center.z += offset.z;
}

// 0x006F8C64 (name is ours)
void collision::Sphere::Set(const nn::math::VEC3& center, f32 radius)
{
    m_Center = center;
    m_Radius = radius;
}
} // namespace collision
