#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv06Travel @ 0x008CC148
// vtable 0x008F1F3C (vptr 0x008F1F44), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv06Travel : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv06Travel> 0x008201F0)
    SoundTv06Travel() : m_HasVoice(1) {}
    // 0x002A5CD8
    // 0x002A5CC8 (deleting dtor)
    virtual ~SoundTv06Travel();
    virtual void Start() override; // 0x002A56D0 slot 0x08
    virtual void OnLoaded() override; // 0x002A5C98 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x002A5740 slot 0x10

    u8 m_HasVoice; // 0xE1 the narration plays (every second program)
};
ASSERT_OFFSET(SoundTv06Travel, m_HasVoice, 0xE1);
ASSERT_SIZE(SoundTv06Travel, 0xE4);
