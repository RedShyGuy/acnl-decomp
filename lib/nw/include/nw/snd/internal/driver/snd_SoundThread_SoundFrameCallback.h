#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"

// Called at the start and the end of each frame of the sound thread (the class name and
// OnBeginSoundFrame are from the reference symbols).
// RTTI N2nw3snd8internal6driver11SoundThread18SoundFrameCallbackE @ 0x008D0A64
class nw::snd::internal::driver::SoundThread::SoundFrameCallback
{
public:
    // (inline)
    SoundFrameCallback() {}
    virtual ~SoundFrameCallback() {} // slot 0x00

    virtual void OnBeginSoundFrame() = 0; // slot 0x08
    virtual void OnEndSoundFrame(); // 0x004CBA14 slot 0x0C (name is ours)

    ut::LinkListNode m_Link; // 0x4
};
