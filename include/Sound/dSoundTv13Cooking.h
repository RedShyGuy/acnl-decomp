#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv13Cooking @ 0x008CC3F4
// vtable 0x008F2E90 (vptr 0x008F2E98), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv13Cooking : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv13Cooking> 0x008203C8)
    SoundTv13Cooking() : m_Volume(1.0f), m_Scene(-1) {}
    // 0x002BE804
    // 0x002BE7F4 (deleting dtor)
    virtual ~SoundTv13Cooking();
    virtual void Start() override; // 0x002BDB7C slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002BDBFC slot 0x10
    void SetScene(s8 scene, s8 variant); // 0x002BE75C
    // a sound of the cooking (the music stays)
    DECOMP_ALWAYS_INLINE void PlayEffect(u32 soundId);

    f32 m_Volume; // 0xE4 of the music
    s8 m_Scene;   // 0xE8 the scene of the cook (sequence variables 0 and 1)
};
ASSERT_OFFSET(SoundTv13Cooking, m_Volume, 0xE4);
ASSERT_OFFSET(SoundTv13Cooking, m_Scene, 0xE8);
ASSERT_SIZE(SoundTv13Cooking, 0xEC);
