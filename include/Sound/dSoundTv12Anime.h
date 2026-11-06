#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv12Anime @ 0x008CBE08
// vtable 0x008F08F0 (vptr 0x008F08F8), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv12Anime : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv12Anime> 0x00820118)
    SoundTv12Anime() : m_Phase(0), m_MuteFrames(0), m_IsMuted(0), m_IsCrash1(0) {}
    // 0x002796EC
    // 0x002796DC (deleting dtor)
    virtual ~SoundTv12Anime();
    virtual void Start() override; // 0x00279328 slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002793A8 slot 0x10

    u8 m_Phase;      // 0xE1 0 run, 1 jump, 2 crash
    u8 m_MuteFrames; // 0xE2 the track 6 of the music fades out over them
    u8 m_IsMuted;    // 0xE3
    u8 m_IsCrash1;   // 0xE4 the crash sound changes with each jump
};
ASSERT_OFFSET(SoundTv12Anime, m_Phase, 0xE1);
ASSERT_OFFSET(SoundTv12Anime, m_MuteFrames, 0xE2);
ASSERT_OFFSET(SoundTv12Anime, m_IsMuted, 0xE3);
ASSERT_OFFSET(SoundTv12Anime, m_IsCrash1, 0xE4);
ASSERT_SIZE(SoundTv12Anime, 0xE8);
