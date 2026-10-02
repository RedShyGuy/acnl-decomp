#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv16SingCM @ 0x008CC160
// vtable 0x008F1F74 (vptr 0x008F1F7C), offset_to_top 0, 5 entries
class SoundTv16SingCM : public ::SoundTvBase
{
public:
    SoundTv16SingCM(); // ctor candidate(s) 0x008202B8 (unverified)
    virtual ~SoundTv16SingCM(); // 0x002A64E0 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002A64D0 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002A6454 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002A64C0 slot 0x0C | virtual slot, introduced by SoundTvBase
};
