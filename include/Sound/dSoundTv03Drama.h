#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv03Drama @ 0x008CBDF0
// vtable 0x008F08B8 (vptr 0x008F08C0), offset_to_top 0, 5 entries
class SoundTv03Drama : public ::SoundTvBase
{
public:
    SoundTv03Drama(); // ctor candidate(s) 0x00820064 (unverified)
    virtual ~SoundTv03Drama(); // 0x00279078 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00279068 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00278EB4 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00279058 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x00278F28 slot 0x10 | virtual slot, introduced by SoundTvBase
};
