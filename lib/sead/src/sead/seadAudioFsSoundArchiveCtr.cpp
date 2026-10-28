#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"
#include "nw/snd/snd_FsSoundArchive.h"
#include "sead/seadAudioFsSoundArchiveCtr.h"

namespace sead {
// ctor candidate(s) 0x0054A2D8 (unverified)
sead::AudioFsSoundArchiveCtr::AudioFsSoundArchiveCtr()
{
}

// 0x0074DB94 slot 0x00 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioFsSoundArchiveCtr::vf_0x00()
{
}

// 0x0074DB48 slot 0x04 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioFsSoundArchiveCtr::vf_0x04()
{
}

// 0x0054B6D8 slot 0x08
// 0x0054B668 (deleting dtor)
// 0x00778D44 (thunk)
// 0x00778CD4 (deleting thunk)
sead::AudioFsSoundArchiveCtr::~AudioFsSoundArchiveCtr()
{
    // (only the base is destroyed)
}

// 0x0054B5BC slot 0x10 | slot vf_0x10 of sead::AudioSoundArchiveBaseCtr
void sead::AudioFsSoundArchiveCtr::open(const void*)
{
}

} // namespace sead
