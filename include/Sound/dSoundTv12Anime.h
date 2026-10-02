#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv12Anime @ 0x008CBE08
// vtable 0x008F08F0 (vptr 0x008F08F8), offset_to_top 0, 5 entries
class SoundTv12Anime : public ::SoundTvBase
{
public:
    SoundTv12Anime(); // ctor candidate(s) 0x00820118 (unverified)
    virtual ~SoundTv12Anime(); // 0x002796EC slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002796DC slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00279328 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002793A8 slot 0x10 | virtual slot, introduced by SoundTvBase
};
