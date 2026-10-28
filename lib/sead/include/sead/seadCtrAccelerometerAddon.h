#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAccelerometerAddon.h"

namespace sead {
// RTTI N4sead21CtrAccelerometerAddonE @ 0x008D1E84
// vtable 0x00906568 (vptr 0x00906570), offset_to_top 0, 5 entries
class CtrAccelerometerAddon : public ::sead::AccelerometerAddon
{
public:
    CtrAccelerometerAddon(); // ctor candidate(s) 0x0054B400 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D6D8 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AccelerometerAddon::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D68C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098351C (guard 0x00983518)
        static const RuntimeTypeInfo::Derive<AccelerometerAddon> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0054B450 slot 0x08 | virtual slot, introduced by sead::CtrAccelerometerAddon
    virtual void vf_0x0C(); // 0x0054B44C slot 0x0C | virtual slot, introduced by sead::CtrAccelerometerAddon
    virtual void vf_0x10(); // 0x0054B31C slot 0x10 | virtual slot, introduced by sead::CtrAccelerometerAddon
    CtrAccelerometerAddon(sead::Controller*); // 0x0054B400 | nintendogs:bytes [tier A]
};
} // namespace sead
