#include "sead/seadAudioSoundArchiveBaseCtr.h"
#include "nw/snd/snd_MemorySoundArchive.h"
#include "sead/seadAudioMemorySoundArchiveCtr.h"

namespace sead {
// 0x0054D9F0 slot 0x08
// 0x0054D9AC (deleting dtor)
// 0x00778ED0 (thunk)
// 0x00778E8C (deleting thunk)
sead::AudioMemorySoundArchiveCtr::~AudioMemorySoundArchiveCtr()
{
    if (IsAvailable()) {
        Finalize();
    }
}

// 0x0054D970 slot 0x10 | nintendogs:bytes
bool sead::AudioMemorySoundArchiveCtr::open(const void* arg)
{
    Initialize(arg);
    return true;
}

// 0x0054D984 slot 0x14 (name is ours)
void sead::AudioMemorySoundArchiveCtr::close()
{
    if (IsAvailable()) {
        Finalize();
    }
}

} // namespace sead
