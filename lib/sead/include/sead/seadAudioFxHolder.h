#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead13AudioFxHolderE @ 0x008D15E4
// The effect of the sound library behind an AudioFx (an interface; the names of the slots are ours,
// after the functions of the effects they call).
class AudioFxHolder
{
public:
    AudioFxHolder() {}
    virtual ~AudioFxHolder() {}
    virtual u32 getRequiredMemSize() = 0; // slot 0x08
    // param: the parameters of the effect
    virtual bool setParam(const void* param) = 0; // slot 0x0C
    virtual bool assignWorkBuffer(uptr buffer, u32 size) = 0; // slot 0x10
    virtual void releaseWorkBuffer() = 0; // slot 0x14
    // the effect
    virtual void* getFx() = 0; // slot 0x18
};
} // namespace sead
