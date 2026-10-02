#pragma once

#include "decomp.h"
#include "Sound/dSoundObjHold.h"

// RTTI 18SoundObjSymbolTree @ 0x008CC8F8
// vtable 0x008F4B4C (vptr 0x008F4B54), offset_to_top 0, 14 entries
class SoundObjSymbolTree : public ::SoundObjHold
{
public:
    SoundObjSymbolTree(); // ctor candidate(s) 0x002E9610 (unverified)
    virtual ~SoundObjSymbolTree(); // 0x002E9694 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002E9640 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x002E93C8 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x002E93F0 slot 0x10 | virtual slot, introduced by SoundObjBase
};
