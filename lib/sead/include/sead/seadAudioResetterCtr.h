#pragma once

#include "decomp.h"
#include "sead/seadAudioResetter.h"

namespace sead {
// RTTI N4sead16AudioResetterCtrE @ 0x008D1AFC
// vtable 0x00905E14 (vptr 0x00905E1C), offset_to_top 0, 11 entries
// The resetter of the 3DS: fades the master volume out and stops (reset) or keeps (stop) the sounds;
// the end of a reset restores the volume and resumes the sounds (member names are ours).
class AudioResetterCtr : public ::sead::AudioResetter
{
public:
    // the states of the reset and of the stop
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_FADE = 1,  // the volume goes to 0
        STATE_DONE = 2,
    };

    AudioResetterCtr(); // 0x0012CAA4
    virtual ~AudioResetterCtr(); // 0x005468E4 slot 0x00
    // 0x005468E0 slot 0x04 (deleting dtor)
    // (the original falls into AudioResetter::initialize with a nop)
    virtual void initialize(AudioMgr* mgr); // 0x0054177C slot 0x08
    virtual void calc(); // 0x005466E4 slot 0x0C
    virtual void startReset(int fadeFrames); // 0x00546804 slot 0x10
    virtual bool isResetting() const; // 0x0074C7AC slot 0x14
    virtual bool isResetReady(); // 0x0074C780 slot 0x18
    virtual void endReset(); // 0x0054662C slot 0x1C
    virtual void startStop(int fadeFrames); // 0x0054689C slot 0x20
    virtual bool isStopping() const; // 0x0074C7FC slot 0x24
    virtual bool isStopReady(); // 0x0074C7D0 slot 0x28

    State mResetState;   // 0x08
    State mStopState;    // 0x09
    f32 mMasterVolume;   // 0x0C, before the reset
};
ASSERT_SIZE(AudioResetterCtr, 0x10);
} // namespace sead
