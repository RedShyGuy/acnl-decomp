#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadProjection.h"

namespace sead {
// RTTI N4sead21PerspectiveProjectionE @ 0x008D1ED8
// vtable 0x00906624 (vptr 0x0090662C), offset_to_top 0, 8 entries
class PerspectiveProjection : public ::sead::Projection
{
public:
    PerspectiveProjection(); // ctor candidate(s) 0x0054B4D8 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D9C4 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Projection::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D8F0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983534 (guard 0x00983530)
        static const RuntimeTypeInfo::Derive<Projection> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0054B5B8 slot 0x08 | virtual slot, introduced by sead::Projection
    virtual void vf_0x0C(); // 0x0054B5B4 slot 0x0C | virtual slot, introduced by sead::Projection
    virtual void vf_0x10(); // 0x0074D8E8 slot 0x10 | virtual slot, introduced by sead::Projection
    virtual void vf_0x14(); // 0x0074D7DC slot 0x14 | virtual slot, introduced by sead::Projection
    virtual void vf_0x1C(); // 0x0074D93C slot 0x1C | virtual slot, introduced by sead::Projection
};
} // namespace sead
