#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 14SoundTv01Color @ 0x008CBDE4
// vtable 0x008F089C (vptr 0x008F08A4), offset_to_top 0, 5 entries
class SoundTv01Color : public ::SoundTvBase
{
public:
    SoundTv01Color(); // ctor candidate(s) 0x0082000C (unverified)
    virtual ~SoundTv01Color(); // 0x00278EB0 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x00278EA0 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x00278E24 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x0C(); // 0x00278E90 slot 0x0C | virtual slot, introduced by SoundTvBase
};
