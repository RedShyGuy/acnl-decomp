#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundDataManager.h"

namespace sead {
// RTTI N4sead20AudioSoundDataMgrCtrE @ 0x008D1E34
// vtable 0x009063F4 (vptr 0x009063FC), offset_to_top 0, 6 entries
// vtable 0x00906414 (vptr 0x0090641C), offset_to_top -12, 5 entries
class AudioSoundDataMgrCtr : public ::nw::snd::SoundDataManager
{
public:
    virtual ~AudioSoundDataMgrCtr(); // 0x0054A6B0 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::DisposeCallback
    virtual void vf_0x04(); // 0x0054A634 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
    void loadData(unsigned, unsigned, unsigned, nw::snd::SoundHeap*); // 0x0012CB74 | nintendogs:bytes [tier A]
    AudioSoundDataMgrCtr(); // 0x001332A0 | nintendogs:bytes [tier A]
    void connectSoundHeap(sead::AudioSoundHeapCtr*); // 0x0054A24C | nintendogs:bytes [tier A]
    void getSoundArchive() const; // 0x0074CF8C | nintendogs:callgraph [tier A]
};
} // namespace sead
