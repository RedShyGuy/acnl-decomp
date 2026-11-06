#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv04Song @ 0x008CBAE0
// vtable 0x008EF800 (vptr 0x008EF808), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv04Song : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv04Song> 0x0081FE1C)
    SoundTv04Song() : m_Verse(-1), m_Variant(0) {}
    // 0x00247E84
    // 0x00247E74 (deleting dtor)
    virtual ~SoundTv04Song();
    virtual void Start() override; // 0x00247CD8 slot 0x08
    virtual void OnLoaded() override; // 0x00247E64 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x00247D54 slot 0x10

    s16 m_Verse;   // 0xE2 the sequence variable 0 of the music (1..4)
    s32 m_Variant; // 0xE4 the two takes of each verse by turns
};
ASSERT_OFFSET(SoundTv04Song, m_Verse, 0xE2);
ASSERT_OFFSET(SoundTv04Song, m_Variant, 0xE4);
ASSERT_SIZE(SoundTv04Song, 0xE8);
