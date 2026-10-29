#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
class AudioMgr;

// RTTI N4sead19AudioResourceLoaderE @ 0x008D1C58
// What the AudioMgr loads its sound archive with (an interface).
class AudioResourceLoader
{
public:
    AudioResourceLoader() {}
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
        // 0x009834E4 (guard 0x009834E0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioResourceLoader() {}
    // (the names of the slots are ours)
    virtual void initialize(AudioMgr* mgr) = 0; // slot 0x10
    // after the subsets: the buffers of the player
    virtual void setupPlayer() = 0; // slot 0x14
    virtual void finalize() = 0; // slot 0x18
};
} // namespace sead
