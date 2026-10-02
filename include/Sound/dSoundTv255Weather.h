#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 17SoundTv255Weather @ 0x008CC6F4
// vtable 0x008F3E30 (vptr 0x008F3E38), offset_to_top 0, 5 entries
class SoundTv255Weather : public ::SoundTvBase
{
public:
    SoundTv255Weather(); // ctor candidate(s) 0x0082054C (unverified)
    virtual ~SoundTv255Weather(); // 0x002D3F70 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002D3F60 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002D3D78 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002D3F48 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002D3DFC slot 0x10 | virtual slot, introduced by SoundTvBase
};
