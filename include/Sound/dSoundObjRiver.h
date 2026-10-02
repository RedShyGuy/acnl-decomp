#pragma once

#include "decomp.h"
#include "Sound/dSoundObjBase.h"

// RTTI 13SoundObjRiver @ 0x008CBAC8
// vtable 0x008EF7C0 (vptr 0x008EF7C8), offset_to_top 0, 7 entries
class SoundObjRiver : public ::SoundObjBase
{
public:
    SoundObjRiver(); // ctor candidate(s) 0x00247190 (unverified)
    virtual ~SoundObjRiver(); // 0x00247224 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002471D0 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x00246EF0 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x0C(); // 0x0024714C slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x00246F80 slot 0x10 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x14(); // 0x00246F68 slot 0x14 | virtual slot, introduced by SoundObjBase
};
