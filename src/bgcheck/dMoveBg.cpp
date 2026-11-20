#include "bgcheck/dMoveBg.h"

namespace bgcheck {
// 0x0061532C
bgcheck::MoveBg::MoveBg() : m_IsActive(false), m_User(0)
{
    m_Angle.value = 0;
}

// 0x00615384 slot 0x00
// 0x00615360 (deleting dtor)
bgcheck::MoveBg::~MoveBg()
{
    // (only the members are destroyed)
}

// 0x00615270 slot 0x08
void bgcheck::MoveBg::OnTouch(const nn::math::VEC3* /* direction */, void* /* object */, u32 /* flags */,
                              f32 /* radius */)
{
    // empty (in the original too)
}

// 0x0061526C slot 0x0C
void bgcheck::MoveBg::OnGuard(const nn::math::VEC3* /* contact */, u32 /* flags */, void* /* object */)
{
    // empty (in the original too)
}

// 0x00615274
void bgcheck::MoveBg::Set(const nn::math::VEC3& base, const nn::math::VEC3& size, const math::Angle16& angle,
                          u32 group, u32 mask, u32 user, f32 scale)
{
    m_User = user;
    m_Angle.value = 0;
    math::Angle16 quarter;
    quarter.value = static_cast<u16>(((angle.value - 0x2000) & 0xC000) - 0x4000);
    m_Size = size;
    if ((static_cast<u32>(-quarter.value) << 17) != 0) {
        // a turn by 90 or 270 degrees
        m_Size.x = size.z;
        m_Size.z = size.x;
    }
    m_Angle = quarter;
    m_Box.SetFromBase(base, m_Size.x * scale, m_Size.z * scale, m_Size.y * scale);
    Body::Set(&m_Box, group, mask);
    AddMoveBg(this);
    m_IsActive = true;
}

// 0x00615458
void bgcheck::MoveBg::SetBox(const nn::math::VEC3& base, const nn::math::VEC3& size)
{
    m_Box.SetFromBase(base, size.x, size.z, size.y);
}

} // namespace bgcheck
