#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv04Song @ 0x008CBAE0
// vtable 0x008EF800 (vptr 0x008EF808), offset_to_top 0, 5 entries
class SoundTv04Song : public ::SoundTvBase
{
public:
    SoundTv04Song(); // ctor candidate(s) 0x0081FE1C (unverified)
    virtual ~SoundTv04Song(); // 0x00247E84 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00247E74 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00247CD8 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00247E64 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x00247D54 slot 0x10 | virtual slot, introduced by SoundTvBase
};
