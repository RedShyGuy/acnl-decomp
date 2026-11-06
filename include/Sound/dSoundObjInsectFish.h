#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 18SoundObjInsectFish @ 0x008CC8EC
// vtable 0x008F4B0C (vptr 0x008F4B14), offset_to_top 0, 14 entries
// An insect or a fish in the world: the general sounds (escape, move) become the sound of its kind (member names are ours).
class SoundObjInsectFish : public ::SoundObj<1>
{
public:
    SoundObjInsectFish(); // 0x002E92B4
    // 0x002E9378
    // 0x002E9324 (deleting dtor)
    virtual ~SoundObjInsectFish();
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002E91C0 slot 0x10
    virtual sead::SoundHandle* StartSound(u32 soundId) override; // 0x002E9298 slot 0x1C
    virtual sead::SoundHandle* HoldSound(u32 soundId) override; // 0x002E91A4 slot 0x24

    // the sound of the kind for a general sound
    u32 ConvertSound(u32 soundId) const; // 0x002E8AC4

    u16 m_Kind;      // 0x5C (item::ToInsectFishID)
    f32 _60;         // 0x60
    bool m_IsMoving; // 0x64 variable 13 of the sequence is 1
};
ASSERT_SIZE(SoundObjInsectFish, 0x68);
