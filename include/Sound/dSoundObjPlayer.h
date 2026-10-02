#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 14SoundObjPlayer @ 0x008CBDC0
// vtable 0x008F0800 (vptr 0x008F0808), offset_to_top 0, 14 entries
class SoundObjPlayer : public ::SoundObj<4>
{
public:
    SoundObjPlayer(); // ctor candidate(s) 0x002785A0 (unverified)
    virtual ~SoundObjPlayer(); // 0x00278700 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x00278608 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x08(); // 0x00278354 slot 0x08 | virtual slot, introduced by SoundObjBase
    virtual void vf_0x0C(); // 0x0027853C slot 0x0C | virtual slot, introduced by SoundObjBase
    virtual void vf_0x10(); // 0x00278448 slot 0x10 | virtual slot, introduced by SoundObjBase
};
