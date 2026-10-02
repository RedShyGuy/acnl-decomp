#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 11SoundObjNpc @ 0x008CB354
// vtable 0x008ECDC4 (vptr 0x008ECDCC), offset_to_top 0, 14 entries
class SoundObjNpc : public ::SoundObj<2>
{
public:
    SoundObjNpc(); // ctor candidate(s) 0x001DA5D8 (unverified)
    virtual ~SoundObjNpc(); // 0x001DA718 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x001DA668 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x001DA258 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x0C(); // 0x001DA56C slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x001DA400 slot 0x10 | virtual slot, introduced by SoundObjBase
};
