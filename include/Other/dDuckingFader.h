#pragma once

#include "decomp.h"
#include "Sound/dSoundFaderEx.h"

// RTTI 12DuckingFader @ 0x008CB5FC
// vtable 0x008EE418 (vptr 0x008EE420), offset_to_top 0, 5 entries
class DuckingFader : public ::SoundFaderEx
{
public:
    DuckingFader(); // ctor candidate(s) 0x0012F968 (unverified)
    virtual void vf_0x00(); // 0x0014049C slot 0x00 | virtual slot, introduced by SoundFader
    virtual void vf_0x04(); // 0x00200028 slot 0x04 | virtual slot, introduced by SoundFader
    virtual void vf_0x0C(); // 0x001FFFC8 slot 0x0C | virtual slot, introduced by SoundFader
};
