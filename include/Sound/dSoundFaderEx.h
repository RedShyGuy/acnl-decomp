#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"

// RTTI 12SoundFaderEx @ 0x008CB80C
// vtable 0x008EE81C (vptr 0x008EE824), offset_to_top 0, 5 entries
class SoundFaderEx : public ::SoundFader
{
public:
    SoundFaderEx(); // ctor candidate(s) 0x00135F64 (unverified)
    virtual void vf_0x00(); // 0x001404A0 slot 0x00 | virtual slot, introduced by SoundFader
    virtual void vf_0x04(); // 0x0020AA84 slot 0x04 | virtual slot, introduced by SoundFader
    virtual void vf_0x08(); // 0x0012F988 slot 0x08 | virtual slot, introduced by SoundFader
    virtual void vf_0x0C(); // 0x0020A9D4 slot 0x0C | virtual slot, introduced by SoundFader
    virtual void vf_0x10(); // 0x007132C8 slot 0x10 | virtual slot, introduced by SoundFader
};
