#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 21SoundObjFurnitureInst @ 0x008CCD40
// vtable 0x008F658C (vptr 0x008F6594), offset_to_top 0, 16 entries
class SoundObjFurnitureInst : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureInst(); // ctor candidate(s) 0x0032CD78 (unverified)
    virtual ~SoundObjFurnitureInst(); // 0x0032CDF0 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x0032CD98 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x0C(); // 0x0032CD18 slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x3C(); // 0x002E9DAC slot 0x3C | virtual slot, introduced by SoundObjFurniture
};
