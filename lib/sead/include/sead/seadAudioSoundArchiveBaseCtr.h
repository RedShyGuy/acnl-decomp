#pragma once

#include "decomp.h"

namespace sead {
// RTTI N4sead24AudioSoundArchiveBaseCtrE @ 0x008D1F44
// vtable 0x009067E0 (vptr 0x009067E8), offset_to_top 0, 6 entries
class AudioSoundArchiveBaseCtr
{
public:
    AudioSoundArchiveBaseCtr(); // ctor address unknown
    virtual void vf_0x00(); // 0x0074E278 slot 0x00 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x04(); // 0x0074E22C slot 0x04 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x08(); // 0x0054D63C slot 0x08 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x0C(); // 0x0054D638 slot 0x0C | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void open(const void*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
