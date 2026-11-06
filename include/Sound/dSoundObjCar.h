#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 11SoundObjCar @ 0x008CB348
// vtable 0x008ECD84 (vptr 0x008ECD8C), offset_to_top 0, 14 entries
// A sound source with three sounds and a flag that only the game uses (member names are ours).
class SoundObjCar : public ::SoundObj<3>
{
public:
    SoundObjCar(); // 0x001C0CE4
    // 0x001DA1C8
    // 0x001DA134 (deleting dtor)
    virtual ~SoundObjCar();

    bool _8C; // 0x8C
};
ASSERT_SIZE(SoundObjCar, 0x90);
