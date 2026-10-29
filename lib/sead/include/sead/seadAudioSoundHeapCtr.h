#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHeap.h"
#include "sead/hostio/seadNode.h"

namespace sead {
class Heap;

// RTTI N4sead17AudioSoundHeapCtrE @ 0x008D1BE0
// vtable 0x00906018 (vptr 0x00906020), offset_to_top 0, 3 entries
// vtable 0x0090602C (vptr 0x00906034), offset_to_top -32, 1 entries
// The sound heap of the archive player on a sead heap (member names are ours).
class AudioSoundHeapCtr : public ::nw::snd::SoundHeap, public ::sead::hostio::Node
{
public:
    // size 0: all of the heap
    AudioSoundHeapCtr(u32 size, Heap* heap); // 0x0012CAD4
    virtual ~AudioSoundHeapCtr(); // 0x00548BB0 slot 0x00
    // 0x00548B64 slot 0x04 (deleting dtor)

    void setSoundDataManagement(nw::snd::SoundDataManager& dataMgr, nw::snd::SoundArchive& archive); // 0x00548B58 | nintendogs:bytes [tier A]

    u8* mBuffer;                          // 0x24
    nw::snd::SoundDataManager* mDataMgr;  // 0x28
    nw::snd::SoundArchive* mArchive;      // 0x2C
};
ASSERT_SIZE(AudioSoundHeapCtr, 0x30);
} // namespace sead
