#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"
#include "Sound/dSoundObj.h"

// RTTI 17SoundObjExtVolume @ 0x008CC6C4
// vtable 0x008F3D34 (vptr 0x008F3D3C), offset_to_top 0, 14 entries
// A sound source with a volume of its own for each of its two sounds that can fade (member names are ours).
class SoundObjExtVolume : public ::SoundObj<2>
{
public:
    SoundObjExtVolume(); // 0x002D237C
    // 0x002D247C
    // 0x002D23E8 (deleting dtor)
    virtual ~SoundObjExtVolume();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002D2198 slot 0x08
    virtual void SetPosition(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x002D2260 slot 0x10
    virtual void SetSoundVolume(f32 volume, s32 index) override; // 0x002D2238 slot 0x34

    void SetVolume(s32 index, f32 volume, s32 frames); // 0x002D21E8

    sead::CheckedArray<SoundFader, 2> m_Faders; // 0x74
};
ASSERT_SIZE(SoundObjExtVolume, 0x9C);
