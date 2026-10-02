#pragma once

#include "decomp.h"
#include "Sound/dSoundObjFurniture.h"

// RTTI 22SoundObjFurnitureOrgel @ 0x008CCE5C
// vtable 0x008F6CA0 (vptr 0x008F6CA8), offset_to_top 0, 16 entries
class SoundObjFurnitureOrgel : public ::SoundObjFurniture
{
public:
    SoundObjFurnitureOrgel(); // ctor address unknown
    virtual ~SoundObjFurnitureOrgel(); // 0x00332C14 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x00332BA8 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x0C(); // 0x00332AA8 slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x3C(); // 0x003328F4 slot 0x3C | virtual slot, introduced by SoundObjFurniture
};
