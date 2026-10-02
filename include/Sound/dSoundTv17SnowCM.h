#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv17SnowCM @ 0x008CC16C
// vtable 0x008F1F90 (vptr 0x008F1F98), offset_to_top 0, 5 entries
class SoundTv17SnowCM : public ::SoundTvBase
{
public:
    SoundTv17SnowCM(); // ctor candidate(s) 0x00820310 (unverified)
    virtual ~SoundTv17SnowCM(); // 0x002A6570 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002A6560 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002A64E4 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002A6550 slot 0x0C | virtual slot, introduced by SoundTvBase
};
