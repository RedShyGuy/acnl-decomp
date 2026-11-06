#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv08Variety @ 0x008CC3E8
// vtable 0x008F2E74 (vptr 0x008F2E7C), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv08Variety : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv08Variety> 0x00820368)
    SoundTv08Variety() : m_Reaction(0), m_MusicFrame(0) {}
    // 0x002BDB78
    // 0x002BDB68 (deleting dtor)
    virtual ~SoundTv08Variety();
    virtual void Start() override; // 0x002BD6C0 slot 0x08
    virtual void OnLoaded() override; // 0x002BDB58 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x002BD738 slot 0x10

    u16 m_Reaction;   // 0xE2 the reaction of the audience (sequence variables 0..2 of the music)
    u16 m_MusicFrame; // 0xE4 the frames of the music (BGM1 at 154, BGM2 at 1050, again from 1350)
};
ASSERT_OFFSET(SoundTv08Variety, m_Reaction, 0xE2);
ASSERT_OFFSET(SoundTv08Variety, m_MusicFrame, 0xE4);
ASSERT_SIZE(SoundTv08Variety, 0xE8);
