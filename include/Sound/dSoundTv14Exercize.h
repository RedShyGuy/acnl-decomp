#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 17SoundTv14Exercize @ 0x008CC6E8
// vtable 0x008F3E14 (vptr 0x008F3E1C), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv14Exercize : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv14Exercize> 0x008204F0)
    SoundTv14Exercize() : m_Part(0) {}
    // 0x002D3D74
    // 0x002D3D64 (deleting dtor)
    virtual ~SoundTv14Exercize();
    virtual void Start() override; // 0x002D3AF0 slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x002D3C50 slot 0x10
    // the music of the part on a handle; the other handle fades out
    void SetPart(u8 part, u32 index, u32 soundId); // 0x002D3B64

    u8 m_Part; // 0xE1 the part of the exercises (1..8)
};
ASSERT_OFFSET(SoundTv14Exercize, m_Part, 0xE1);
ASSERT_SIZE(SoundTv14Exercize, 0xE4);
