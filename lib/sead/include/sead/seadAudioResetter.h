#pragma once

#include "decomp.h"

namespace sead {
class AudioMgr;

// RTTI N4sead13AudioResetterE @ 0x008D15EC
// vtable 0x00905318 (vptr 0x00905320), offset_to_top 0, 11 entries
// Resets (and stops) the audio: tells all subsets of the AudioMgr (names of the slots and members are
// ours, isResetting after the reference symbols).
class AudioResetter
{
public:
    AudioResetter(); // 0x00132F80
    virtual ~AudioResetter(); // 0x005418BC slot 0x00
    // 0x005418B8 slot 0x04 (deleting dtor)
    virtual void initialize(AudioMgr* mgr); // 0x00541780 slot 0x08
    virtual void calc() = 0; // slot 0x0C
    virtual void startReset(int fadeFrames); // 0x005417E8 slot 0x10
    virtual bool isResetting() const; // 0x0074B164 slot 0x14
    virtual bool isResetReady(); // 0x0074B0F8 slot 0x18
    virtual void endReset(); // 0x00541788 slot 0x1C
    virtual void startStop(int fadeFrames); // 0x00541850 slot 0x20
    virtual bool isStopping() const; // 0x0074B244 slot 0x24
    virtual bool isStopReady(); // 0x0074B1D8 slot 0x28

    AudioMgr* mMgr; // 0x4
};
} // namespace sead
