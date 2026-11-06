#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv05Quiz @ 0x008CBAEC
// vtable 0x008EF81C (vptr 0x008EF824), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv05Quiz : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv05Quiz> 0x0081FE80)
    SoundTv05Quiz() : m_Round(0) {}
    // 0x00248378
    // 0x00248368 (deleting dtor)
    virtual ~SoundTv05Quiz();
    virtual void Start() override; // 0x00247E88 slot 0x08
    virtual void UpdateProgram(s32 frame) override; // 0x00247EFC slot 0x10

    s8 m_Round; // 0xE1 two rounds by turns
};
ASSERT_OFFSET(SoundTv05Quiz, m_Round, 0xE1);
ASSERT_SIZE(SoundTv05Quiz, 0xE4);
