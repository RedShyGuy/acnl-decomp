#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 14SoundObjHaniwa @ 0x008CBDB4
// vtable 0x008F07C0 (vptr 0x008F07C8), offset_to_top 0, 14 entries
class SoundObjHaniwa : public ::SoundObj<2>
{
public:
    SoundObjHaniwa(); // ctor candidate(s) 0x00278108 (unverified)
    virtual ~SoundObjHaniwa(); // 0x002781D8 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002781C8 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
    virtual void vf_0x0C(); // 0x00277E00 slot 0x0C | virtual slot, introduced by SoundObjBase
};
