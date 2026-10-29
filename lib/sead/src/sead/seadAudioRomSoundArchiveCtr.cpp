#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"
#include "nw/snd/snd_RomSoundArchive.h"
#include "sead/seadAudioRomSoundArchiveCtr.h"

namespace sead {
// 0x0054BD7C slot 0x08
// 0x0054BD0C (deleting dtor)
// 0x00778E20 (thunk)
// 0x00778DB0 (deleting thunk)
sead::AudioRomSoundArchiveCtr::~AudioRomSoundArchiveCtr()
{
    // (only the base is destroyed)
}


// 0x0054BC60 slot 0x10
bool sead::AudioRomSoundArchiveCtr::open(const void* arg)
{
    return openDevice_(arg);
}
} // namespace sead
