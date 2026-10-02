#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv18UfoCM @ 0x008CBE14
// vtable 0x008F090C (vptr 0x008F0914), offset_to_top 0, 5 entries
class SoundTv18UfoCM : public ::SoundTvBase
{
public:
    SoundTv18UfoCM(); // ctor candidate(s) 0x00820180 (unverified)
    virtual ~SoundTv18UfoCM(); // 0x00279C58 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00279C48 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002796F0 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00279B98 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x00279778 slot 0x10 | virtual slot, introduced by SoundTvBase
};
