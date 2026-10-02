#pragma once

#include "decomp.h"
#include "Sound/dSoundObjHold.h"

// RTTI 12SoundObjFall @ 0x008CB820
// vtable 0x008EE85C (vptr 0x008EE864), offset_to_top 0, 14 entries
class SoundObjFall : public ::SoundObjHold
{
public:
    SoundObjFall(); // ctor address unknown
    virtual ~SoundObjFall(); // 0x0020B610 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x0020B5BC slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x0020B574 slot 0x08 | virtual slot, introduced by SoundObjBase
};
