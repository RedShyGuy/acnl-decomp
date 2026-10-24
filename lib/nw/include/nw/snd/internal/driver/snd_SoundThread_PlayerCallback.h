#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"

// What a player does in the sound thread (the class name is from the reference symbols, the function
// names are ours; StreamSoundPlayer: Update, UpdateBuffer, Stop).
// RTTI N2nw3snd8internal6driver11SoundThread14PlayerCallbackE @ 0x008D0A5C
class nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    // (inline)
    PlayerCallback() {}
    virtual ~PlayerCallback() {} // slot 0x00

    // after the commands of the frame
    virtual void OnUpdateFrame() = 0; // slot 0x08
    // before the commands of the frame
    virtual void OnUpdateBuffer(); // 0x004CB61C slot 0x0C
    // the sound thread ends
    virtual void OnShutdown() = 0; // slot 0x10

    ut::LinkListNode m_Link; // 0x4
};
