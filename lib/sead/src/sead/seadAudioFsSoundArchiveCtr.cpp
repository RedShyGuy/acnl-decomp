#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"
#include "nw/snd/snd_FsSoundArchive.h"
#include "sead/seadAudioFsSoundArchiveCtr.h"

namespace sead {
// 0x0054B6D8 slot 0x08
// 0x0054B668 (deleting dtor)
// 0x00778D44 (thunk)
// 0x00778CD4 (deleting thunk)
sead::AudioFsSoundArchiveCtr::~AudioFsSoundArchiveCtr()
{
    // (only the base is destroyed)
}


// 0x0054B5BC slot 0x10
bool sead::AudioFsSoundArchiveCtr::open(const void* arg)
{
    return openDevice_(arg);
}
} // namespace sead

// the type information of the classes below AudioSoundArchiveBaseCtr
// 0x00829464 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::AudioSoundArchiveBaseCtr>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
