#pragma once

#include "decomp.h"
#include "Sound/dSoundTvBase.h"

// RTTI 17SoundTv14Exercize @ 0x008CC6E8
// vtable 0x008F3E14 (vptr 0x008F3E1C), offset_to_top 0, 5 entries
class SoundTv14Exercize : public ::SoundTvBase
{
public:
    SoundTv14Exercize(); // ctor candidate(s) 0x008204F0 (unverified)
    virtual ~SoundTv14Exercize(); // 0x002D3D74 slot 0x00 | slot vf_0x00 of SoundTvBase
    // 0x002D3D64 slot 0x04 | slot vf_0x04 of SoundTvBase (deleting dtor)
    virtual void vf_0x08(); // 0x002D3AF0 slot 0x08 | virtual slot, introduced by SoundTvBase
    virtual void vf_0x10(); // 0x002D3C50 slot 0x10 | virtual slot, introduced by SoundTvBase
};
