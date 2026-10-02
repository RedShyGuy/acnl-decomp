#include "nw/snd/snd_SoundDataManager.h"
#include "sead/seadAudioSoundDataMgrCtr.h"

namespace sead {
// 0x0054A6B0 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::DisposeCallback
sead::AudioSoundDataMgrCtr::~AudioSoundDataMgrCtr()
{
}

// 0x0054A634 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::DisposeCallback
void sead::AudioSoundDataMgrCtr::vf_0x04()
{
}

// 0x0012CB74 | nintendogs:bytes [tier A]
void sead::AudioSoundDataMgrCtr::loadData(unsigned, unsigned, unsigned, nw::snd::SoundHeap*)
{
}

// 0x001332A0 | nintendogs:bytes [tier A]
sead::AudioSoundDataMgrCtr::AudioSoundDataMgrCtr()
{
}

// 0x0054A24C | nintendogs:bytes [tier A]
void sead::AudioSoundDataMgrCtr::connectSoundHeap(sead::AudioSoundHeapCtr*)
{
}

// 0x0074CF8C | nintendogs:callgraph [tier A]
void sead::AudioSoundDataMgrCtr::getSoundArchive() const
{
}

} // namespace sead
