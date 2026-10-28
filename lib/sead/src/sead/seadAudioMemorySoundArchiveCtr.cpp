#include "sead/seadAudioSoundArchiveBaseCtr.h"
#include "nw/snd/snd_MemorySoundArchive.h"
#include "sead/seadAudioMemorySoundArchiveCtr.h"

namespace sead {
// ctor candidate(s) 0x0054A580 (unverified)
// (the argument of the nw base is not checked yet; sead is not decompiled)
sead::AudioMemorySoundArchiveCtr::AudioMemorySoundArchiveCtr() : nw::snd::MemorySoundArchive(false)
{
}

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
void sead::AudioMemorySoundArchiveCtr::open(const void*)
{
}

// 0x0054D984 slot 0x14 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
void sead::AudioMemorySoundArchiveCtr::vf_0x14()
{
}

} // namespace sead
