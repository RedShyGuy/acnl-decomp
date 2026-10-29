#pragma once

#include "decomp.h"
#include "sead/seadDelegateBase.h"
#include "sead/seadIDelegateR.h"
#include "sead/seadNew.h"

namespace sead {
// Instantiations found in the binary:
//   sead::DelegateR<AcNpc, bool>  typeinfo 0x008D2350  vtable 0x00907028
//   sead::DelegateR<AcNpcSp, bool>  typeinfo 0x008D235C  vtable 0x00907038
//   sead::DelegateR<AcNpcSpMaiko, bool>  typeinfo 0x008D2338  vtable 0x00907008
//   sead::DelegateR<MoveFromTalkReceptBase, bool>  typeinfo 0x008D2344  vtable 0x00907018
//   sead::DelegateR<SeaDemoCtrl, bool>  typeinfo 0x008D232C  vtable 0x00906FF8
//
// Calls a member function of an object; clone copies the delegate onto a heap.
template <typename T0, typename T1>
class DelegateR : public DelegateBase<T0, T1 (T0::*)(), IDelegateR<T1>>
{
public:
    typedef DelegateBase<T0, T1 (T0::*)(), IDelegateR<T1>> Base;

    DelegateR() : Base() {}
    DelegateR(T0* object, typename Base::MethodType method) : Base(object, method) {}

    virtual T1 invoke()
    {
        if (this->mObject != nullptr && this->mMethod != nullptr) {
            return (this->mObject->*this->mMethod)();
        }
        return T1();
    }
    virtual IDelegateR<T1>* clone(Heap* heap) const { return new (heap, 4) DelegateR(*this); }
};
} // namespace sead
