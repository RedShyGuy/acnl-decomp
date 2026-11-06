#pragma once

#include "decomp.h"
#include "Sound/dSoundObjBase.h"

// RTTI 13SoundObjRiver @ 0x008CBAC8
// vtable 0x008EF7C0 (vptr 0x008EF7C8), offset_to_top 0, 7 entries
// A piece of the river: it only delivers volume and pan for the one sound of SoundRiverMgr. While the fade of SoundMgr is
// down the position stays frozen and it moves back when the fade comes up again (member names are ours).
class SoundObjRiver : public ::SoundObjBase
{
public:
    SoundObjRiver(); // 0x00247190
    // 0x00247224
    // 0x002471D0 (deleting dtor)
    virtual ~SoundObjRiver(); // slot 0x00
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x00246EF0 slot 0x08
    virtual void Stop() override; // 0x0024714C slot 0x0C
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x00246F80 slot 0x10
    virtual bool UpdateChanged() override; // 0x00246F68 slot 0x14

    u32 m_Index;               // 0x44 in SoundRiverMgr
    bool m_IsInRange;          // 0x48
    bool m_IsFrozen;           // 0x49
    nn::math::VEC3 m_FrozenPos; // 0x4C
};
ASSERT_OFFSET(SoundObjRiver, m_Index, 0x44);
ASSERT_OFFSET(SoundObjRiver, m_FrozenPos, 0x4C);
ASSERT_SIZE(SoundObjRiver, 0x58);
