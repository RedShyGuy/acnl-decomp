#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 18SoundTv21LaundryCM @ 0x008CC910
// vtable 0x008F4BCC (vptr 0x008F4BD4), offset_to_top 0, 5 entries
class SoundTv21LaundryCM : public ::SoundTvBase
{
public:
    SoundTv21LaundryCM(); // ctor candidate(s) 0x008205B0 (unverified)
    virtual ~SoundTv21LaundryCM(); // 0x002EA2B8 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002EA2A8 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002EA020 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002EA094 slot 0x10 | virtual slot, introduced by SoundTvBase
};
