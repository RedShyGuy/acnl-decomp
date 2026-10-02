#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv08Variety @ 0x008CC3E8
// vtable 0x008F2E74 (vptr 0x008F2E7C), offset_to_top 0, 5 entries
class SoundTv08Variety : public ::SoundTvBase
{
public:
    SoundTv08Variety(); // ctor candidate(s) 0x00820368 (unverified)
    virtual ~SoundTv08Variety(); // 0x002BDB78 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002BDB68 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002BD6C0 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x002BDB58 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002BD738 slot 0x10 | virtual slot, introduced by SoundTvBase
};
