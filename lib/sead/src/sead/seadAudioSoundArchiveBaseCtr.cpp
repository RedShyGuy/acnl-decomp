#include "sead/seadAudioSoundArchiveBaseCtr.h"

namespace sead {
// ctor address unknown
sead::AudioSoundArchiveBaseCtr::AudioSoundArchiveBaseCtr()
{
}

// 0x0054D63C slot 0x08
// 0x0054D638 (deleting dtor)
sead::AudioSoundArchiveBaseCtr::~AudioSoundArchiveBaseCtr()
{
    // empty (in the original too)
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void sead::AudioSoundArchiveBaseCtr::open(const void*)
{
}

// 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
void sead::AudioSoundArchiveBaseCtr::vf_0x14()
{
}

} // namespace sead
