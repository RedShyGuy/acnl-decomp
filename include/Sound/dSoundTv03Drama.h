#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv03Drama @ 0x008CBDF0
// vtable 0x008F08B8 (vptr 0x008F08C0), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv03Drama : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv03Drama> 0x00820064)
    SoundTv03Drama() : m_Episode(0) {}
    // 0x00279078
    // 0x00279068 (deleting dtor)
    virtual ~SoundTv03Drama();
    virtual void Start() override; // 0x00278EB4 slot 0x08
    virtual void OnLoaded() override; // 0x00279058 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x00278F28 slot 0x10

    s8 m_Episode; // 0xE1 two episodes by turns
};
ASSERT_OFFSET(SoundTv03Drama, m_Episode, 0xE1);
ASSERT_SIZE(SoundTv03Drama, 0xE4);
