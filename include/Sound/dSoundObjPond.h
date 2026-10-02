#pragma once

#include "decomp.h"
#include "Sound/dSoundObjHold.h"

// RTTI 12SoundObjPond @ 0x008CB838
// vtable 0x008EE8DC (vptr 0x008EE8E4), offset_to_top 0, 14 entries
class SoundObjPond : public ::SoundObjHold
{
public:
    SoundObjPond(); // ctor address unknown
    virtual ~SoundObjPond(); // 0x0020BA00 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x0020B9AC slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x0020B968 slot 0x08 | virtual slot, introduced by SoundObjBase
};
