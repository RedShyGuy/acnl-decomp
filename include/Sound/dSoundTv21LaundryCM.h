#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 18SoundTv21LaundryCM @ 0x008CC910
// vtable 0x008F4BCC (vptr 0x008F4BD4), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv21LaundryCM : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv21LaundryCM> 0x008205B0)
    SoundTv21LaundryCM() : m_Scene(-1) {}
    // 0x002EA2B8
    // 0x002EA2A8 (deleting dtor)
    virtual ~SoundTv21LaundryCM();
    virtual void Start() override; // 0x002EA020 slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002EA094 slot 0x10
    DECOMP_NOINLINE void SetScene(s8 scene, s8 variant); // 0x002EA210

    s8 m_Scene; // 0xE1 the scene of the voice (sequence variable 0)
};
ASSERT_OFFSET(SoundTv21LaundryCM, m_Scene, 0xE1);
ASSERT_SIZE(SoundTv21LaundryCM, 0xE4);
