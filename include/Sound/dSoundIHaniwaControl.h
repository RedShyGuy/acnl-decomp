#pragma once

#include "decomp.h"

// RTTI 19SoundIHaniwaControl @ 0x008CCA80
// What the gyroids dance to: the timing of the music that is playing (the slot names are ours).
class SoundIHaniwaControl
{
public:
    virtual ~SoundIHaniwaControl() {}
    virtual f32 GetTempo() = 0;
    virtual s16 GetBeat() = 0;
    virtual s32 GetTicksPerBeat() = 0;
    virtual s32 GetFramesPerBeat() = 0;
    virtual s16 GetTickInBeat() = 0;
    virtual s32 GetFramesToBeat() = 0;
    virtual u32 GetRandom() = 0;
};
