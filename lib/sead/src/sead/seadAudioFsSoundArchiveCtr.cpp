#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"
#include "nw/snd/snd_FsSoundArchive.h"
#include "sead/seadAudioFsSoundArchiveCtr.h"

namespace sead {
// ctor candidate(s) 0x0054A2D8 (unverified)
sead::AudioFsSoundArchiveCtr::AudioFsSoundArchiveCtr()
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

// the type information of the classes below AudioSoundArchiveBaseCtr
// 0x00829464 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioSoundArchiveBaseCtr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
