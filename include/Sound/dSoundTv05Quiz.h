#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv05Quiz @ 0x008CBAEC
// vtable 0x008EF81C (vptr 0x008EF824), offset_to_top 0, 5 entries
class SoundTv05Quiz : public ::SoundTvBase
{
public:
    SoundTv05Quiz(); // ctor candidate(s) 0x0081FE80 (unverified)
    virtual ~SoundTv05Quiz(); // 0x00248378 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00248368 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00247E88 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x00247EFC slot 0x10 | virtual slot, introduced by SoundTvBase
};
