#pragma once

#include "decomp.h"
#include "sead/seadListNode.h"
#include "sead/seadController.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerBase.h"
#include "sead/seadIDisposer.h"

namespace sead {
// RTTI N4sead21ControllerWrapperBaseE @ 0x008D1E64
// vtable 0x00906534 (vptr 0x0090653C), offset_to_top 0, 7 entries
// vtable 0x00906558 (vptr 0x00906560), offset_to_top -308, 2 entries
// A controller that follows another one (in its list of wrappers; member names are ours).
class ControllerWrapperBase : public ::sead::ControllerBase, public ::sead::IDisposer
{
public:
    ControllerWrapperBase(); // 0x0054B210
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074D5D8 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return ControllerBase::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074D58C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983514 (guard 0x00983510)
        static const RuntimeTypeInfo::Derive<ControllerBase> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~ControllerWrapperBase(); // 0x0054B2C4 slot 0x08
    // 0x0054B264 slot 0x0C (deleting dtor)
    // the state from the controller
    virtual void calc(u32 prevHold, bool prevPointerOn) = 0; // slot 0x10
    // (the original falls into setIdleBase_ with a nop)
    virtual void setIdle_(); // 0x0054415C slot 0x14
    virtual bool isIdle_(); // 0x0054B20C slot 0x18

    // follows controller (copyRepeat: with its repeat settings; name is ours)
    void setController(Controller* controller, bool copyRepeat); // 0x0054B17C

    Controller* mController; // 0x144
    bool mIsEnable;          // 0x148
    ListNode mNode;          // 0x14C, in Controller::mWrappers
    u8 field_0x154[0x20];    // 0x154 (not set by the constructors)
};
ASSERT_SIZE(ControllerWrapperBase, 0x174);
} // namespace sead
