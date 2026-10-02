#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 12SoundObjHold @ 0x008CB82C
// vtable 0x008EE89C (vptr 0x008EE8A4), offset_to_top 0, 14 entries
class SoundObjHold : public ::SoundObj<1>
{
public:
    SoundObjHold(); // ctor candidate(s) 0x0020B83C (unverified)
    virtual ~SoundObjHold(); // 0x0020B918 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x0020B8C4 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x10(); // 0x0020B690 slot 0x10 | virtual slot, introduced by SoundObjBase
};
