#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 18SoundObjInsectFish @ 0x008CC8EC
// vtable 0x008F4B0C (vptr 0x008F4B14), offset_to_top 0, 14 entries
class SoundObjInsectFish : public ::SoundObj<1>
{
public:
    SoundObjInsectFish(); // ctor candidate(s) 0x002E92B4 (unverified)
    virtual ~SoundObjInsectFish(); // 0x002E9378 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002E9324 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x10(); // 0x002E91C0 slot 0x10 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x1C(); // 0x002E9298 slot 0x1C | virtual slot, introduced by SoundObj<1>
    virtual void vf_0x24(); // 0x002E91A4 slot 0x24 | virtual slot, introduced by SoundObj<1>
};
