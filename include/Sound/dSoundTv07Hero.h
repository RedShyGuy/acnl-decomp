#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv07Hero @ 0x008CBAF8
// vtable 0x008EF838 (vptr 0x008EF840), offset_to_top 0, 5 entries
class SoundTv07Hero : public ::SoundTvBase
{
public:
    SoundTv07Hero(); // ctor candidate(s) 0x0081FEDC (unverified)
    virtual ~SoundTv07Hero(); // 0x00248744 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00248734 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x0024837C slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00248724 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x00248400 slot 0x10 | virtual slot, introduced by SoundTvBase
};
