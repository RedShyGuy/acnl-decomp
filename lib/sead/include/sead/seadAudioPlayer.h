#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead11AudioPlayerE @ 0x008D13E8
class AudioPlayer
{
public:
    AudioPlayer(); // ctor address unknown
    // (the runtime type info slots 0x00/0x04 and the destructor 0x08/0x0C, as in AudioPlayerCtr 0x00905678)
    virtual void vf_0x00() = 0;
    virtual void vf_0x04() = 0;
    virtual ~AudioPlayer() {}
};
} // namespace sead
