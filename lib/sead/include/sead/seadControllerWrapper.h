#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerWrapperBase.h"

namespace sead {
// RTTI N4sead17ControllerWrapperE @ 0x008D1C00
// vtable 0x00906038 (vptr 0x00906040), offset_to_top 0, 7 entries
// vtable 0x0090605C (vptr 0x00906064), offset_to_top -308, 2 entries
// A controller with the pad bits of another one in another order (member names are ours).
class ControllerWrapper : public ::sead::ControllerWrapperBase
{
public:
    ControllerWrapper(); // 0x00548E10
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C86C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControllerWrapperBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C820 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009834BC (guard 0x009834B8)
        static const RuntimeTypeInfo::Derive<ControllerWrapperBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    // (the original falls into the destructor of ControllerWrapperBase with a nop)
    virtual ~ControllerWrapper(); // 0x0054B2C0 slot 0x08
    // 0x00548E4C slot 0x0C (deleting dtor)
    virtual void calc(u32 prevHold, bool prevPointerOn); // 0x00548CA0 slot 0x10

    // the default order: bit i is bit i of the controller
    static const u8 s_DefaultPadConfig[28];

    u8 mPadConfig[28]; // 0x174, the bit of the controller of each pad bit
};
ASSERT_SIZE(ControllerWrapper, 0x190);
} // namespace sead
