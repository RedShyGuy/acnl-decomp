#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// the holder of an AudioFxObjectCtr: its effect and the parameters of a DSP effect (only the slots
// AudioSystemCtr::appendEffect calls; names are ours)
class AudioFxObjectHolder
{
public:
    virtual void vf_0x00() = 0;
    virtual void vf_0x04() = 0;
    virtual void vf_0x08() = 0;
    virtual void vf_0x0C() = 0;
    virtual void vf_0x10() = 0;
    virtual void vf_0x14() = 0;
    virtual void* getFx() = 0; // slot 0x18
    virtual const void* getParam() = 0; // slot 0x1C
};

// The other kind of effect of AudioSystem::appendEffect (slot 0x24). Only its type information and that
// function are in the binary: the name of AudioFxObject is from the type information of the classes
// below it; the slots and the members are what appendEffect uses (names are ours).
class AudioFxObject
{
public:
    virtual void vf_0x00() = 0;
    // whether the class is the one of typeInfo or derives from it (slot 0x04)
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const
    {
        return typeInfo == getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983404 (guard 0x00983400)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }

    AudioFxObjectHolder* mHolder; // 0x04
};

// the effect of the 3DS of that kind (name is ours; see AudioFxCtr::Type for mType)
class AudioFxObjectCtr : public AudioFxObject
{
public:
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioFxObject::isKindOf(typeInfo);
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834B4 (guard 0x009834B0)
        static const RuntimeTypeInfo::Derive<AudioFxObject> s_TypeInfo;
        return &s_TypeInfo;
    }

    u8 field_0x08[0x10]; // 0x08
    u8 mType;            // 0x18
};
} // namespace sead
