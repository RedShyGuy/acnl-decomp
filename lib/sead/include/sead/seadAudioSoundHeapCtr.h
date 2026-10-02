#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHeap.h"
#include "sead/hostio/seadNode.h"

namespace sead {
// RTTI N4sead17AudioSoundHeapCtrE @ 0x008D1BE0
// vtable 0x00906018 (vptr 0x00906020), offset_to_top 0, 3 entries
// vtable 0x0090602C (vptr 0x00906034), offset_to_top -32, 1 entries
class AudioSoundHeapCtr : public ::nw::snd::SoundHeap, public ::sead::hostio::Node
{
public:
    AudioSoundHeapCtr(); // ctor candidate(s) 0x0012CAD4 (unverified)
    virtual ~AudioSoundHeapCtr(); // 0x00548BB0 slot 0x00 | slot vf_0x00 of nw::snd::SoundHeap
    virtual void vf_0x04(); // 0x00548B64 slot 0x04 | virtual slot, introduced by nw::snd::SoundHeap
    void setSoundDataManagement(nw::snd::SoundDataManager&, nw::snd::SoundArchive&); // 0x00548B58 | nintendogs:callgraph [tier A]
};
} // namespace sead
