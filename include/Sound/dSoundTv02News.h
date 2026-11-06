#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv02News @ 0x008CBAD4
// vtable 0x008EF7E4 (vptr 0x008EF7EC), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv02News : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv02News> 0x0081FDBC)
    SoundTv02News() : m_Part(-1) {}
    // 0x001DBC08
    // 0x00247CC8 (deleting dtor)
    virtual ~SoundTv02News();
    virtual void Start() override; // 0x002478A4 slot 0x08
    virtual void OnLoaded() override; // 0x00247CB8 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x00247918 slot 0x10

    s8 m_Part; // 0xE1 the part of the program, changes each round (0..2)
};
ASSERT_OFFSET(SoundTv02News, m_Part, 0xE1);
ASSERT_SIZE(SoundTv02News, 0xE4);
