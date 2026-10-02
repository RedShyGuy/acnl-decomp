#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 22SoundObjFurnitureAudio @ 0x008CCE50
// vtable 0x008F6C58 (vptr 0x008F6C60), offset_to_top 0, 16 entries
class SoundObjFurnitureAudio : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureAudio(); // ctor candidate(s) 0x00332268 (unverified)
    virtual ~SoundObjFurnitureAudio(); // 0x003322F8 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x00332280 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x0C(); // 0x00332208 slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x38(); // 0x0033202C slot 0x38 | virtual slot, introduced by SoundObjFurniture
    virtual void vf_0x3C(); // 0x003320EC slot 0x3C | virtual slot, introduced by SoundObjFurniture
};
