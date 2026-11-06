#pragma once

#include "decomp.h"
#include "Sound/dSoundObj.h"

// RTTI 14SoundObjSimple @ 0x008CBDCC
// vtable 0x008F0840 (vptr 0x008F0848), offset_to_top 0, 14 entries
// A sound object with one sound and nothing else (the class name is from the RTTI).
class SoundObjSimple : public ::SoundObj<1>
{
public:
    SoundObjSimple() {}
    virtual ~SoundObjSimple(); // 0x00278848
    // 0x002787F4 (deleting dtor)
};
ASSERT_SIZE(SoundObjSimple, 0x5C);
