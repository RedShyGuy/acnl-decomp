#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv15AppleCM @ 0x008CC400
// vtable 0x008F2EAC (vptr 0x008F2EB4), offset_to_top 0, 5 entries
class SoundTv15AppleCM : public ::SoundTvBase
{
public:
    SoundTv15AppleCM(); // ctor candidate(s) 0x00820434 (unverified)
    virtual ~SoundTv15AppleCM(); // 0x002BE894 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002BE884 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002BE808 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002BE874 slot 0x0C | virtual slot, introduced by SoundTvBase
};
