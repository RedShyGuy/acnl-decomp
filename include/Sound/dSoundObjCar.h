#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 11SoundObjCar @ 0x008CB348
// vtable 0x008ECD84 (vptr 0x008ECD8C), offset_to_top 0, 14 entries
class SoundObjCar : public ::SoundObj<3>
{
public:
    SoundObjCar(); // ctor candidate(s) 0x001C0CE4 (unverified)
    virtual ~SoundObjCar(); // 0x001DA1C8 slot 0x00 | slot vf_0x00 of SoundObjBase
    // 0x001DA134 slot 0x04 | slot vf_0x04 of SoundObjBase (deleting dtor)
};
