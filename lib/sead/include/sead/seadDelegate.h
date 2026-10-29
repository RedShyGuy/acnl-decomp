#pragma once

#include "decomp.h"
#include "sead/seadDelegateBase.h"
#include "sead/seadIDelegate.h"
#include "sead/seadNew.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Delegate<AcNpcSpResetsan>  typeinfo 0x008D2190  vtable 0x00906CC4
//   sead::Delegate<nfp::Framework>  typeinfo 0x008D219C  vtable 0x00906CD4
//   sead::Delegate<sead::CalculateTask>  typeinfo 0x008D21A8  vtable 0x00906CE4
//   sead::Delegate<sead::DualScreenTask>  typeinfo 0x008D21C0  vtable 0x00906D04
//   sead::Delegate<sead::FaderTaskBase>  typeinfo 0x008D21B4  vtable 0x00906CF4
//   sead::Delegate<sead::MethodTreeNode>  typeinfo 0x008D21CC  vtable 0x00906D14
//   sead::Delegate<sead::UlcdTask>  typeinfo 0x008D21D8  vtable 0x00906D24
//
// Calls a member function of an object; clone copies the delegate onto a heap.
template <typename T0>
class Delegate : public DelegateBase<T0, void (T0::*)(), IDelegate>
{
public:
    typedef DelegateBase<T0, void (T0::*)(), IDelegate> Base;

    Delegate() : Base() {}
    Delegate(T0* object, typename Base::MethodType method) : Base(object, method) {}

    virtual void invoke()
    {
        if (this->mObject != nullptr && this->mMethod != nullptr) {
            (this->mObject->*this->mMethod)();
        }
    }
    virtual IDelegate* clone(Heap* heap) const { return new (heap, 4) Delegate(*this); }
};
} // namespace sead
