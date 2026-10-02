#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 15SoundTv06Travel @ 0x008CC148
// vtable 0x008F1F3C (vptr 0x008F1F44), offset_to_top 0, 5 entries
class SoundTv06Travel : public ::SoundTvBase
{
public:
    SoundTv06Travel(); // ctor candidate(s) 0x008201F0 (unverified)
    virtual ~SoundTv06Travel(); // 0x002A5CD8 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002A5CC8 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002A56D0 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002A5C98 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002A5740 slot 0x10 | virtual slot, introduced by SoundTvBase
};
