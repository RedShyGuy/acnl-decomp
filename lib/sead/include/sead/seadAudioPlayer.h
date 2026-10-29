#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace nw {
namespace snd {
class SoundHandle;
} // namespace snd
} // namespace nw

namespace sead {
// RTTI N4sead11AudioPlayerE @ 0x008D13E8
class AudioPlayer
{
public:
    AudioPlayer() {}
    // (the runtime type info slots 0x00/0x04 and the destructor 0x08/0x0C, as in AudioPlayerCtr 0x00905678)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833C4 (guard 0x009833C0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioPlayer() {}
    // (the names of the slots are ours)
    virtual void initialize() = 0; // slot 0x10
    virtual void finalize() = 0; // slot 0x14
    virtual void calc() = 0; // slot 0x18
    // true if the sound started
    virtual bool startSound(nw::snd::SoundHandle* handle, u32 soundId) = 0; // slot 0x1C
    virtual bool startSound(nw::snd::SoundHandle* handle, const char* label) = 0; // slot 0x20
    virtual bool holdSound(nw::snd::SoundHandle* handle, u32 soundId) = 0; // slot 0x24
    virtual bool holdSound(nw::snd::SoundHandle* handle, const char* label) = 0; // slot 0x28
    virtual u32 getSoundCount() const = 0; // slot 0x2C
    virtual const char* getSoundLabel(u32 soundId) const = 0; // slot 0x30
    virtual u32 getSoundId(const char* label) const = 0; // slot 0x34
};
} // namespace sead
