#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerBase.h"

namespace sead {
// RTTI N4sead10ControllerE @ 0x008D1358
// vtable 0x00904E0C (vptr 0x00904E14), offset_to_top 0, 9 entries
class Controller : public ::sead::ControllerBase
{
public:
    Controller(); // ctor candidate(s) 0x0053E5D0 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00749F1C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControllerBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00749ED0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833A4 (guard 0x009833A0)
        static const RuntimeTypeInfo::Derive<ControllerBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0053E640 slot 0x08 | virtual slot, introduced by sead::Controller
    virtual void vf_0x0C(); // 0x0053E63C slot 0x0C | virtual slot, introduced by sead::Controller
    virtual void vf_0x10(); // 0x0053E490 slot 0x10 | virtual slot, introduced by sead::Controller
    virtual void vf_0x14(); // 0x00749EC8 slot 0x14 | virtual slot, introduced by sead::Controller
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x005440B8 slot 0x1C | virtual slot, introduced by sead::Controller
    virtual void vf_0x20(); // 0x0053E584 slot 0x20 | virtual slot, introduced by sead::Controller
};
} // namespace sead
