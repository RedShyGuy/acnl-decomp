#include "sead/hostio/seadNode.h"
#include "nw/snd/snd_SoundHeap.h"
#include "sead/seadAudioSoundHeapCtr.h"

namespace sead {
// ctor candidate(s) 0x0012CAD4 (unverified)
sead::AudioSoundHeapCtr::AudioSoundHeapCtr()
{
}

// 0x00548BB0 slot 0x00 | slot vf_0x00 of nw::snd::SoundHeap
sead::AudioSoundHeapCtr::~AudioSoundHeapCtr()
{
}

// 0x00548B64 slot 0x04 | virtual slot, introduced by nw::snd::SoundHeap
void sead::AudioSoundHeapCtr::vf_0x04()
{
}

// 0x00548B58 | nintendogs:callgraph [tier A]
void sead::AudioSoundHeapCtr::setSoundDataManagement(nw::snd::SoundDataManager&, nw::snd::SoundArchive&)
{
}

} // namespace sead
