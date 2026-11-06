#pragma once

#include "decomp.h"
#include "Sound/dSoundObjHold.h"

// RTTI 12SoundObjPond @ 0x008CB838
// vtable 0x008EE8DC (vptr 0x008EE8E4), offset_to_top 0, 14 entries
// A pond (SE_ENV_POND).
class SoundObjPond : public ::SoundObjHold
{
public:
    // 0x0020BA00
    // 0x0020B9AC (deleting dtor)
    virtual ~SoundObjPond();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x0020B968 slot 0x08
};
