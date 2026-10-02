#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"
#include "nw/snd/snd_RomSoundArchive.h"
#include "sead/seadAudioRomSoundArchiveCtr.h"

namespace sead {
// ctor candidate(s) 0x0054A430 (unverified)
sead::AudioRomSoundArchiveCtr::AudioRomSoundArchiveCtr()
{
}

// 0x0074DE94 slot 0x00 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioRomSoundArchiveCtr::vf_0x00()
{
}

// 0x0074DE48 slot 0x04 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioRomSoundArchiveCtr::vf_0x04()
{
}

// 0x0054BD7C slot 0x08 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioRomSoundArchiveCtr::vf_0x08()
{
}

// 0x0054BD0C slot 0x0C | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioRomSoundArchiveCtr::vf_0x0C()
{
}

// 0x0054BC60 slot 0x10 | slot vf_0x10 of sead::AudioSoundArchiveBaseCtr
void sead::AudioRomSoundArchiveCtr::open(const void*)
{
}

} // namespace sead
