#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 17SoundTv255Weather @ 0x008CC6F4
// vtable 0x008F3E30 (vptr 0x008F3E38), offset_to_top 0, 5 entries
// A TV program (the class name is from the RTTI, everything else is ours).
class SoundTv255Weather : public ::SoundTvBase
{
public:
    // (embedded in SoundTvHandle::Create<SoundTv255Weather> 0x0082054C)
    SoundTv255Weather() : m_Day(0), m_Song(0), m_Report(0) {}
    // 0x002D3F70
    // 0x002D3F60 (deleting dtor)
    virtual ~SoundTv255Weather();
    virtual void Start() override; // 0x002D3D78 slot 0x08
    virtual void OnLoaded() override; // 0x002D3F48 slot 0x0C
    virtual void UpdateProgram(s32 frame) override; // 0x002D3DFC slot 0x10

    u8 m_Day;    // 0xE1 a new song every 7 programs
    u8 m_Song;   // 0xE2 SE_TV_255_KK_SWING + m_Song (0..2)
    u8 m_Report; // 0xE3 two forecasts by turns (0..2)
};
ASSERT_OFFSET(SoundTv255Weather, m_Day, 0xE1);
ASSERT_OFFSET(SoundTv255Weather, m_Song, 0xE2);
ASSERT_OFFSET(SoundTv255Weather, m_Report, 0xE3);
ASSERT_SIZE(SoundTv255Weather, 0xE4);
