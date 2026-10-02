#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv20JuiceCM @ 0x008CC40C
// vtable 0x008F2EC8 (vptr 0x008F2ED0), offset_to_top 0, 5 entries
class SoundTv20JuiceCM : public ::SoundTvBase
{
public:
    SoundTv20JuiceCM(); // ctor candidate(s) 0x0082048C (unverified)
    virtual ~SoundTv20JuiceCM(); // 0x002BEA6C slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002BEA5C slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002BE898 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002BE910 slot 0x10 | virtual slot, introduced by SoundTvBase
};
