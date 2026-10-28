#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadResource.h"

namespace sead {
// RTTI N4sead14DirectResourceE @ 0x008D16EC
// vtable 0x009057A4 (vptr 0x009057AC), offset_to_top 0, 6 entries
class DirectResource : public ::sead::Resource
{
public:
    DirectResource(); // ctor candidate(s) 0x005448A8 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074BE14 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Resource::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074BDC0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098346C (guard 0x00983468)
        static const RuntimeTypeInfo::Derive<Resource> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x005448FC slot 0x08 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x0C(); // 0x005448CC slot 0x0C | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x10(); // 0x0074BE0C slot 0x10 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x14(); // 0x00544890 slot 0x14 | virtual slot, introduced by sead::DirectResource
};
} // namespace sead
