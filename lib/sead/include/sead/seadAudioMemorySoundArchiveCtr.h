#pragma once

#include "decomp.h"
#include "nw/snd/snd_MemorySoundArchive.h"
#include "sead/seadAudioSoundArchiveBaseCtr.h"

namespace sead {
// RTTI N4sead26AudioMemorySoundArchiveCtrE @ 0x008D1F58
// vtable 0x00906800 (vptr 0x00906808), offset_to_top 0, 6 entries
// vtable 0x00906820 (vptr 0x00906828), offset_to_top -8, 6 entries
class AudioMemorySoundArchiveCtr : public ::sead::AudioSoundArchiveBaseCtr, public ::nw::snd::MemorySoundArchive
{
public:
    AudioMemorySoundArchiveCtr(); // ctor candidate(s) 0x0054A580 (unverified)
    virtual void vf_0x00(); // 0x0074E320 slot 0x00 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x04(); // 0x0074E2D4 slot 0x04 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x08(); // 0x0054D9F0 slot 0x08 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x0C(); // 0x0054D9AC slot 0x0C | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void open(const void*); // 0x0054D970 slot 0x10 | nintendogs:bytes
    virtual void vf_0x14(); // 0x0054D984 slot 0x14 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
};
} // namespace sead
