#include "bgcheck/dGuardBox.h"

namespace bgcheck {
// 0x006154C0
bgcheck::GuardBox::GuardBox() : m_pCallback(NULL)
{
}

// 0x00317F74 slot 0x00
// 0x006154E0 (deleting dtor)
bgcheck::GuardBox::~GuardBox()
{
    Remove();
}

// 0x00615434 slot 0x0C
void bgcheck::GuardBox::OnGuard(const nn::math::VEC3* contact, u32 flags, void* object)
{
    if (m_pCallback != NULL) {
        m_pCallback(contact, flags, object);
    }
}

// 0x0061546C
void bgcheck::GuardBox::Set(const nn::math::VEC3& base, const nn::math::VEC3& size, u32 group, u32 mask,
                            Callback callback)
{
    math::Angle16 angle;
    angle.value = 0;
    MoveBg::Set(base, size, angle, group, mask, 0, 1.0f);
    m_pCallback = callback;
}

} // namespace bgcheck
