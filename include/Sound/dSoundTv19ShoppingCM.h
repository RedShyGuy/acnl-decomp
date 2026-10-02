#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 19SoundTv19ShoppingCM @ 0x008CCA88
// vtable 0x008F5370 (vptr 0x008F5378), offset_to_top 0, 5 entries
class SoundTv19ShoppingCM : public ::SoundTvBase
{
public:
    SoundTv19ShoppingCM(); // ctor candidate(s) 0x00820610 (unverified)
    virtual ~SoundTv19ShoppingCM(); // 0x002FFF64 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002FFF54 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002FF22C slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002FFA60 slot 0x10 | virtual slot, introduced by SoundTvBase
};
