#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "nw/snd/snd_RomSoundArchive.h"
#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"

namespace sead {
// RTTI N4sead23AudioRomSoundArchiveCtrE @ 0x008D1F20
// vtable 0x009066F8 (vptr 0x00906700), offset_to_top 0, 6 entries
// vtable 0x00906718 (vptr 0x00906720), offset_to_top -8, 6 entries
class AudioRomSoundArchiveCtr : public ::sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>
{
public:
    explicit AudioRomSoundArchiveCtr(Heap* heap) : AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>(0, heap) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074DE94 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioSoundArchiveBaseCtr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074DE48 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983554 (guard 0x00983550)
        static const RuntimeTypeInfo::Derive<AudioSoundArchiveBaseCtr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioRomSoundArchiveCtr(); // 0x0054BD7C slot 0x08
    virtual bool open(const void* arg); // 0x0054BC60 slot 0x10
};
} // namespace sead
