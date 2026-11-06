#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv11Talk @ 0x008CBB04
// vtable 0x008EF854 (vptr 0x008EF85C), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv11Talk : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv11Talk> 0x0081FF48)
    SoundTv11Talk() : m_Volume(1.0f), m_Scene(-1) {}
    // 0x0024928C
    // 0x0024927C (deleting dtor)
    virtual ~SoundTv11Talk();
    virtual void Start() override; // 0x00248748 slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002487C8 slot 0x10
    void SetScene(s8 scene, s8 variant); // 0x002491E4

    f32 m_Volume; // 0xE4 of the music
    s8 m_Scene;   // 0xE8 the scene of the talk (sequence variables 0 and 1), 56 the laughter at the end
};
ASSERT_OFFSET(SoundTv11Talk, m_Volume, 0xE4);
ASSERT_OFFSET(SoundTv11Talk, m_Scene, 0xE8);
ASSERT_SIZE(SoundTv11Talk, 0xEC);
