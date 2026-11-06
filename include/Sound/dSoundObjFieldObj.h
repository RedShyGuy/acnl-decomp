#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 16SoundObjFieldObj @ 0x008CC3DC
// vtable 0x008F2E34 (vptr 0x008F2E3C), offset_to_top 0, 14 entries
// A sound source of the field; like SoundObjHold the position stays frozen while the fade of SoundMgr is down,
// SE_OBJ_BELL_JPN gets the type 12 (member names are ours).
class SoundObjFieldObj : public ::SoundObj<1>
{
public:
    SoundObjFieldObj(); // 0x002BD5A4
    // 0x002BD670
    // 0x002BD618 (deleting dtor)
    virtual ~SoundObjFieldObj();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002BD3AC slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002BD3C8 slot 0x10
    virtual sead::SoundHandle* StartSound(u32 soundId) override; // 0x002BD520 slot 0x1C
    virtual sead::SoundHandle* StartSound(u32 soundId, s16 value) override; // 0x002BD558 slot 0x20

    bool m_IsFrozen;            // 0x5C
    nn::math::VEC3 m_FrozenPos; // 0x60
};
ASSERT_OFFSET(SoundObjFieldObj, m_IsFrozen, 0x5C);
ASSERT_SIZE(SoundObjFieldObj, 0x6C);
