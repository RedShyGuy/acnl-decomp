#pragma once

#include "decomp.h"
#include "Sound/dSoundFader.h"

// RTTI 16SoundFaderPlayer @ 0x008CC3D0
// vtable 0x008F2E18 (vptr 0x008F2E20), offset_to_top 0, 5 entries
class SoundFaderPlayer : public ::SoundFader
{
public:
    SoundFaderPlayer(); // ctor candidate(s) 0x0012FA44 (unverified)
    virtual void vf_0x00(); // 0x0013E33C slot 0x00 | virtual slot, introduced by SoundFader
    virtual void vf_0x04(); // 0x002BD3A8 slot 0x04 | virtual slot, introduced by SoundFader
    virtual void vf_0x0C(); // 0x002BD1CC slot 0x0C | virtual slot, introduced by SoundFader
};
