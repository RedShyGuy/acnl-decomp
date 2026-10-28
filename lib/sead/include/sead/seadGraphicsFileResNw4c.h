#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDirectResource.h"

namespace sead {
// RTTI N4sead19GraphicsFileResNw4cE @ 0x008D1E28
// vtable 0x009063D4 (vptr 0x009063DC), offset_to_top 0, 6 entries
class GraphicsFileResNw4c : public ::sead::DirectResource
{
public:
    GraphicsFileResNw4c(); // ctor candidate(s) 0x0054A180 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074CE88 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return DirectResource::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074CE34 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834F4 (guard 0x009834F0)
        static const RuntimeTypeInfo::Derive<DirectResource> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0054A1DC slot 0x08 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x0C(); // 0x0054A1A8 slot 0x0C | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x10(); // 0x0074CE80 slot 0x10 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x14(); // 0x0054A164 slot 0x14 | virtual slot, introduced by sead::DirectResource
};
} // namespace sead
