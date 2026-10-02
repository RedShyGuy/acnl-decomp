#pragma once

#include "decomp.h"
#include "nw/snd/snd_FsSoundArchive.h"
#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"

namespace sead {
// RTTI N4sead22AudioFsSoundArchiveCtrE @ 0x008D1EE4
// vtable 0x0090664C (vptr 0x00906654), offset_to_top 0, 6 entries
// vtable 0x0090666C (vptr 0x00906674), offset_to_top -8, 6 entries
class AudioFsSoundArchiveCtr : public ::sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::FsSoundArchive>
{
public:
    AudioFsSoundArchiveCtr(); // ctor candidate(s) 0x0054A2D8 (unverified)
    virtual void vf_0x00(); // 0x0074DB94 slot 0x00 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x04(); // 0x0074DB48 slot 0x04 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x08(); // 0x0054B6D8 slot 0x08 | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void vf_0x0C(); // 0x0054B668 slot 0x0C | virtual slot, introduced by sead::AudioSoundArchiveBaseCtr
    virtual void open(const void*); // 0x0054B5BC slot 0x10 | slot vf_0x10 of sead::AudioSoundArchiveBaseCtr
};
} // namespace sead
