#pragma once

#include "decomp.h"
#include "Sound/dSoundObjHold.h"

// RTTI 12SoundObjFall @ 0x008CB820
// vtable 0x008EE85C (vptr 0x008EE864), offset_to_top 0, 14 entries
// A water fall (SE_ENV_WATERFALL).
class SoundObjFall : public ::SoundObjHold
{
public:
    // 0x0020B610
    // 0x0020B5BC (deleting dtor)
    virtual ~SoundObjFall();
    virtual void Start(const nn::math::VEC3* world, const nn::math::VEC3* relative) override; // 0x0020B574 slot 0x08
};
