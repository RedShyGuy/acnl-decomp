#pragma once

#include "decomp.h"

// RTTI 12SoundObjBase @ 0x008CB818
// vtable 0x008EE838 (vptr 0x008EE840), offset_to_top 0, 7 entries
class SoundObjBase
{
public:
    SoundObjBase(); // ctor candidate(s) 0x0020B4D4 (unverified)
    virtual ~SoundObjBase(); // 0x0020B570 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x0020B56C slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x0020AA88 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x0C(); // 0x0020B498 slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x0020B3E8 slot 0x10 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x14(); // 0x0020B264 slot 0x14 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x18(); // 0x0020AC30 slot 0x18 | virtual slot, introduced by SoundObjBase
};
