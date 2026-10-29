#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "nw/snd/snd_MemorySoundArchive.h"
#include "sead/seadAudioSoundArchiveBaseCtr.h"

namespace sead {
// RTTI N4sead26AudioMemorySoundArchiveCtrE @ 0x008D1F58
// vtable 0x00906800 (vptr 0x00906808), offset_to_top 0, 6 entries
// vtable 0x00906820 (vptr 0x00906828), offset_to_top -8, 6 entries
class AudioMemorySoundArchiveCtr : public ::sead::AudioSoundArchiveBaseCtr, public ::nw::snd::MemorySoundArchive
{
public:
    AudioMemorySoundArchiveCtr() : AudioSoundArchiveBaseCtr(1), nw::snd::MemorySoundArchive(true) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E320 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioSoundArchiveBaseCtr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E2D4 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098357C (guard 0x00983578)
        static const RuntimeTypeInfo::Derive<AudioSoundArchiveBaseCtr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioMemorySoundArchiveCtr(); // 0x0054D9F0 slot 0x08
    // arg: the data of the archive
    virtual bool open(const void* arg); // 0x0054D970 slot 0x10 | nintendogs:bytes
    virtual void close(); // 0x0054D984 slot 0x14
};
ASSERT_SIZE(AudioMemorySoundArchiveCtr, 0x180);
} // namespace sead
