#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerAddon.h"

namespace sead {
// RTTI N4sead18AccelerometerAddonE @ 0x008D1C2C
class AccelerometerAddon : public ::sead::ControllerAddon
{
public:
    AccelerometerAddon(); // ctor address unknown
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControllerAddon::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834D4 (guard 0x009834D0)
        static const RuntimeTypeInfo::Derive<ControllerAddon> s_TypeInfo;
        return &s_TypeInfo;
    }
};
} // namespace sead
