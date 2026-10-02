#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 16SoundTv13Cooking @ 0x008CC3F4
// vtable 0x008F2E90 (vptr 0x008F2E98), offset_to_top 0, 5 entries
class SoundTv13Cooking : public ::SoundTvBase
{
public:
    SoundTv13Cooking(); // ctor candidate(s) 0x008203C8 (unverified)
    virtual ~SoundTv13Cooking(); // 0x002BE804 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002BE7F4 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002BDB7C slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002BDBFC slot 0x10 | virtual slot, introduced by SoundTvBase
};
