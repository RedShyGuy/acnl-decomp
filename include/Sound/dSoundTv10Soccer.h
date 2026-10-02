#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv10Soccer @ 0x008CC154
// vtable 0x008F1F58 (vptr 0x008F1F60), offset_to_top 0, 5 entries
class SoundTv10Soccer : public ::SoundTvBase
{
public:
    SoundTv10Soccer(); // ctor candidate(s) 0x00820250 (unverified)
    virtual ~SoundTv10Soccer(); // 0x002A6450 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002A6440 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002A5CDC slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002A6430 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002A5D5C slot 0x10 | virtual slot, introduced by SoundTvBase
};
