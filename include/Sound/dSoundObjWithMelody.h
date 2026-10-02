#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 18SoundObjWithMelody @ 0x008CC904
// vtable 0x008F4B8C (vptr 0x008F4B94), offset_to_top 0, 14 entries
class SoundObjWithMelody : public ::SoundObj<1>
{
public:
    SoundObjWithMelody(); // ctor candidate(s) 0x002E987C (unverified)
    virtual ~SoundObjWithMelody(); // 0x002E98F4 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002E98E0 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x002E96E4 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x0C(); // 0x002E9834 slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x002E9760 slot 0x10 | virtual slot, introduced by SoundObjBase
};
