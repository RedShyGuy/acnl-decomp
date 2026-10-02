#pragma once

#include "decomp.h"
#include "nw/snd/snd_RomSoundArchive.h"
#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"

namespace sead {
// RTTI N4sead23AudioRomSoundArchiveCtrE @ 0x008D1F20
// vtable 0x009066F8 (vptr 0x00906700), offset_to_top 0, 6 entries
// vtable 0x00906718 (vptr 0x00906720), offset_to_top -8, 6 entries
class AudioRomSoundArchiveCtr : public ::sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>
{
public:
    AudioRomSoundArchiveCtr(); // ctor candidate(s) 0x0054A430 (unverified)
    virtual void vf_0x00(); // 0x0074DE94 slot 0x00 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x04(); // 0x0074DE48 slot 0x04 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x08(); // 0x0054BD7C slot 0x08 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x0C(); // 0x0054BD0C slot 0x0C | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void open(const void*); // 0x0054BC60 slot 0x10 | slot vf_0x10 of sead::AudioSoundArchiveBaseCtr
};
} // namespace sead
