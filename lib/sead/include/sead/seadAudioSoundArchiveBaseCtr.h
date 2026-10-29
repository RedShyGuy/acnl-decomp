#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead24AudioSoundArchiveBaseCtrE @ 0x008D1F44
// vtable 0x009067E0 (vptr 0x009067E8), offset_to_top 0, 6 entries
class AudioSoundArchiveBaseCtr
{
public:
    explicit AudioSoundArchiveBaseCtr(u8 type) : m_ArchiveType(type) {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E278 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E22C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098356C (guard 0x00983568)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioSoundArchiveBaseCtr(); // 0x0054D63C slot 0x08
    // opens the archive (arg: the path or the data; name of open from the reference symbols)
    virtual bool open(const void* arg) = 0; // slot 0x10
    virtual void close() = 0; // slot 0x14

    u8 m_ArchiveType; // 0x04 0 ROM, 1 memory, 2 file system (set by the creation functions; name is ours)
};
ASSERT_SIZE(AudioSoundArchiveBaseCtr, 0x8);
} // namespace sead
