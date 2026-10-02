#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 23SoundObjFurnitureInsect @ 0x008CCF40
// vtable 0x008F72E4 (vptr 0x008F72EC), offset_to_top 0, 16 entries
class SoundObjFurnitureInsect : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureInsect(); // ctor candidate(s) 0x00335B68 (unverified)
    virtual ~SoundObjFurnitureInsect(); // 0x002D3158 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x00335B88 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x3C(); // 0x00335AB4 slot 0x3C | virtual slot, introduced by SoundObjFurniture
};
