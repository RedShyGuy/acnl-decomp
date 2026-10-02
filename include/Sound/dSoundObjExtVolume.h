#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 17SoundObjExtVolume @ 0x008CC6C4
// vtable 0x008F3D34 (vptr 0x008F3D3C), offset_to_top 0, 14 entries
class SoundObjExtVolume : public ::SoundObj<2>
{
public:
    SoundObjExtVolume(); // ctor candidate(s) 0x002D237C (unverified)
    virtual ~SoundObjExtVolume(); // 0x002D247C slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002D23E8 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x002D2198 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x002D2260 slot 0x10 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x34(); // 0x002D2238 slot 0x34 | virtual slot, introduced by SoundObj<2>
};
