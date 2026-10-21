#include "nw/math/math_AABB.h"

namespace nw {
namespace math {
// 0x004D9B4C | fefates:bytes [tier B]
void nw::math::AABB::Set(const nn::math::VEC3* pPoints, unsigned int count)
{
    m_Min = pPoints[0];
    m_Max = pPoints[0];
    for (u32 i = 1; i < count; i++) {
        const nn::math::VEC3& point = pPoints[i];
        if (point.x < m_Min.x) {
            m_Min.x = point.x;
        } else if (point.x > m_Max.x) {
            m_Max.x = point.x;
        }
        if (point.y < m_Min.y) {
            m_Min.y = point.y;
        } else if (point.y > m_Max.y) {
            m_Max.y = point.y;
        }
        if (point.z < m_Min.z) {
            m_Min.z = point.z;
        } else if (point.z > m_Max.z) {
            m_Max.z = point.z;
        }
    }
}

// 0x004D9C08 (name is ours)
void nw::math::AABB::Normalize()
{
    if (m_Min.x > m_Max.x) {
        f32 x = m_Min.x;
        m_Min.x = m_Max.x;
        m_Max.x = x;
    }
    if (m_Min.y > m_Max.y) {
        f32 y = m_Min.y;
        m_Min.y = m_Max.y;
        m_Max.y = y;
    }
    if (m_Min.z > m_Max.z) {
        f32 z = m_Min.z;
        m_Min.z = m_Max.z;
        m_Max.z = z;
    }
}

} // namespace math
} // namespace nw
