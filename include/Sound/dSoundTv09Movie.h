#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv09Movie @ 0x008CBDFC
// vtable 0x008F08D4 (vptr 0x008F08DC), offset_to_top 0, 5 entries
class SoundTv09Movie : public ::SoundTvBase
{
public:
    SoundTv09Movie(); // ctor candidate(s) 0x008200C0 (unverified)
    virtual ~SoundTv09Movie(); // 0x00279324 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00279314 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x0027907C slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002792E4 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002790E8 slot 0x10 | virtual slot, introduced by SoundTvBase
};
