#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 17SoundObjFurniture @ 0x008CC6D0
// vtable 0x008F3D74 (vptr 0x008F3D7C), offset_to_top 0, 16 entries
class SoundObjFurniture : public ::SoundObj<2>
{
public:
    SoundObjFurniture(); // ctor candidate(s) 0x002D3018 (unverified)
    virtual ~SoundObjFurniture(); // 0x002D315C slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002D30B0 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x0C(); // 0x002D2DAC slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x1C(); // 0x002D2C50 slot 0x1C | virtual slot, introduced by SoundObj<2>
    virtual void vf_0x24(); // 0x002D29A0 slot 0x24 | virtual slot, introduced by SoundObj<2>
    virtual void vf_0x38(); // 0x002D250C slot 0x38 | virtual slot, introduced by SoundObjFurniture
    virtual void vf_0x3C(); // 0x002D2AAC slot 0x3C | virtual slot, introduced by SoundObjFurniture
};
