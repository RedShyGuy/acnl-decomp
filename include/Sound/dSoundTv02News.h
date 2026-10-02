#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 13SoundTv02News @ 0x008CBAD4
// vtable 0x008EF7E4 (vptr 0x008EF7EC), offset_to_top 0, 5 entries
class SoundTv02News : public ::SoundTvBase
{
public:
    SoundTv02News(); // ctor candidate(s) 0x0081FDBC (unverified)
    virtual ~SoundTv02News(); // 0x001DBC08 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00247CC8 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002478A4 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00247CB8 slot 0x0C | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x00247918 slot 0x10 | virtual slot, introduced by SoundTvBase
};
