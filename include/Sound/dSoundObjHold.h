#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 12SoundObjHold @ 0x008CB82C
// vtable 0x008EE89C (vptr 0x008EE8A4), offset_to_top 0, 14 entries
// A sound that is held while the object is near (water falls, ponds); like SoundObjRiver the position stays frozen while the
// fade of SoundMgr is down (member names are ours).
class SoundObjHold : public ::SoundObj<1>
{
public:
    SoundObjHold(); // 0x0020B83C
    // 0x0020B918
    // 0x0020B8C4 (deleting dtor)
    virtual ~SoundObjHold();
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x0020B690 slot 0x10

    void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative, u32 soundId); // 0x0020B660

    u32 m_SoundId;              // 0x5C
    bool m_IsFrozen;            // 0x60
    nn::math::VEC3 m_FrozenPos; // 0x64
};
ASSERT_OFFSET(SoundObjHold, m_SoundId, 0x5C);
ASSERT_SIZE(SoundObjHold, 0x70);
