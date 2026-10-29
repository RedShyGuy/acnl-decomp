#pragma once

#include "decomp.h"
#include "sead/seadAudioFx.h"
#include "sead/seadAudioGlobal.h"
#include "sead/seadListImpl.h"

namespace sead {
class AudioFxMemoryMgr;

// The effects with work buffers: setup gives an effect its memory, append its bus (through the
// AudioSystem of the AudioMgr), dispose takes the effects of a memory out (member names are ours).
class AudioFxMgr
{
public:
    AudioFxMgr() : mList() { mList.initOffset(offsetof(AudioFx, mNode)); }

    // the work buffer of fx from memoryMgr (true if fx was set up already)
    bool setup(AudioFx* fx, AudioFxMemoryMgr* memoryMgr, s32 alignment); // 0x0053E260 | nintendogs:callseq [tier A]
    // fx on bus (false if fx is not set up)
    bool append(AudioGlobal::AuxBus bus, AudioFx* fx); // 0x0053E2F4 | nintendogs:bytes-fuzzy [tier B]
    // clears the buses of the effects of memoryMgr and takes them out
    void dispose(AudioFxMemoryMgr* memoryMgr); // 0x0053E348 | nintendogs:callseq [tier A]
    // clears the effects of bus in the AudioSystem (fading out over fadeFrames) and takes the bus from
    // the effects that were on it (inline in dispose)
    void clearBus(AudioGlobal::AuxBus bus, s32 fadeFrames); // 0x00143910 (name is ours)

    OffsetList<AudioFx> mList; // 0x0
};
ASSERT_SIZE(AudioFxMgr, 0x10);
} // namespace sead
