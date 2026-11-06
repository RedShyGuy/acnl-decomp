#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv20JuiceCM @ 0x008CC40C
// vtable 0x008F2EC8 (vptr 0x008F2ED0), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv20JuiceCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv20JuiceCM> 0x0082048C)
    SoundTv20JuiceCM() : m_Volume(1.0f) {}
    // 0x002BEA6C
    // 0x002BEA5C (deleting dtor)
    virtual ~SoundTv20JuiceCM();
    virtual void Start() override; // 0x002BE898 slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002BE910 slot 0x10

    f32 m_Volume; // 0xE4 of the music
};
ASSERT_OFFSET(SoundTv20JuiceCM, m_Volume, 0xE4);
ASSERT_SIZE(SoundTv20JuiceCM, 0xE8);
