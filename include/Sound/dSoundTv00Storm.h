#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv00Storm @ 0x008CBDD8
// vtable 0x008F0880 (vptr 0x008F0888), offset_to_top 0, 5 entries
class SoundTv00Storm : public ::SoundTvBase
{
public:
    SoundTv00Storm(); // ctor candidate(s) 0x0081FFB4 (unverified)
    virtual ~SoundTv00Storm(); // 0x00278E20 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00278E10 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00278D94 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00278E00 slot 0x0C | virtual slot, introduced by SoundTvBase
};
