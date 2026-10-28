#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
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
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074DB94 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioSoundArchiveBaseCtr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074DB48 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098353C (guard 0x00983538)
        static const RuntimeTypeInfo::Derive<AudioSoundArchiveBaseCtr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioFsSoundArchiveCtr(); // 0x0054B6D8 slot 0x08
    virtual void open(const void*); // 0x0054B5BC slot 0x10 | slot vf_0x10 of sead::AudioSoundArchiveBaseCtr
};
} // namespace sead
