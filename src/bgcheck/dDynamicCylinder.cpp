#include "bgcheck/dDynamicCylinder.h"

namespace bgcheck {
// 0x00615040
bgcheck::DynamicCylinder::DynamicCylinder()
{
    // (only the members are constructed)
}

// 0x00615084 slot 0x00
// 0x00615060 (deleting dtor)
bgcheck::DynamicCylinder::~DynamicCylinder()
{
    // (only the members are destroyed)
}

// 0x006F8960 (name is ours)
void bgcheck::DynamicCylinder::Set(const nn::math::VEC3* center, f32 radius, f32 height, u32 group, u32 mask)
{
    m_Cylinder.Set(center, radius, height);
    Body::Set(&m_Cylinder, group, mask);
}

// 0x006F8EF0 (name is ours)
void bgcheck::DynamicCylinder::SetCylinder(const nn::math::VEC3* center, f32 radius, f32 height)
{
    m_Cylinder.Set(center, radius, height);
}

} // namespace bgcheck
