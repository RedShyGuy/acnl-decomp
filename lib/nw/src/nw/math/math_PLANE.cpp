#include <cmath>

#include "nw/math/math_PLANE.h"

namespace nw {
namespace math {
// 0x004D9C54 | fefates:bytes [tier B]
void nw::math::PLANE::Set(const nn::math::VEC3* p0, const nn::math::VEC3* p1, const nn::math::VEC3* p2)
{
    nn::math::VEC3 v0 = *p2 - *p0;
    nn::math::VEC3 v1 = *p1 - *p0;
    nn::math::VEC3 normal(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
    f32 scale = 1.0f / std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    m_Normal.x = normal.x * scale;
    m_Normal.y = normal.y * scale;
    m_Normal.z = normal.z * scale;
    m_D = -(m_Normal.x * p0->x + m_Normal.y * p0->y + m_Normal.z * p0->z);
}

// 0x004D9D0C (name is ours)
nw::math::PLANE::PLANE()
{
}

} // namespace math
} // namespace nw
