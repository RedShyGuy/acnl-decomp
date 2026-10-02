#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 14SoundObjSimple @ 0x008CBDCC
// vtable 0x008F0840 (vptr 0x008F0848), offset_to_top 0, 14 entries
class SoundObjSimple : public ::SoundObj<1>
{
public:
    SoundObjSimple(); // ctor address unknown
    virtual ~SoundObjSimple(); // 0x00278848 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x002787F4 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
};
