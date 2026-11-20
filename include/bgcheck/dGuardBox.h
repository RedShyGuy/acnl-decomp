#pragma once

#include "decomp.h"
#include "bgcheck/dMoveBg.h"

namespace bgcheck {
// a box that calls a function when the movement test touches it (BsNpcMgr has four, the functions come from the
// modules; names are ours)
// RTTI N7bgcheck8GuardBoxE @ 0x008D3F00
// vtable 0x0090BCAC (vptr 0x0090BCB4), offset_to_top 0, 4 entries
class GuardBox : public ::bgcheck::MoveBg
{
public:
    typedef void (*Callback)(const nn::math::VEC3* contact, u32 flags, void* object);

    GuardBox(); // 0x006154C0
    ~GuardBox() override; // 0x00317F74 slot 0x00
    // 0x006154E0 (deleting dtor)
    void OnGuard(const nn::math::VEC3* contact, u32 flags, void* object) override; // 0x00615434 slot 0x0C

    // an unturned box of size on base
    void Set(const nn::math::VEC3& base, const nn::math::VEC3& size, u32 group, u32 mask, Callback callback); // 0x0061546C

    Callback m_pCallback; // 0x48
};
ASSERT_SIZE(GuardBox, 0x4C);
} // namespace bgcheck
