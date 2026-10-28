#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
// RTTI N4sead10ProjectionE @ 0x008D13CC
// vtable 0x00904F20 (vptr 0x00904F28), offset_to_top 0, 8 entries
class Projection
{
public:
    Projection(); // ctor address unknown
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A494 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A1F8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833B4 (guard 0x009833B0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0053F248 slot 0x08 | virtual slot, introduced by sead::Projection
    virtual void vf_0x0C(); // 0x0053F244 slot 0x0C | virtual slot, introduced by sead::Projection
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0074A244 slot 0x18 | virtual slot, introduced by sead::Projection
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
