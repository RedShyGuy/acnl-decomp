#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead11AudioPlayerE @ 0x008D13E8
class AudioPlayer
{
public:
    AudioPlayer(); // ctor address unknown
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
};
} // namespace sead
