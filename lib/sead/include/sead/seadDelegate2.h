#pragma once

#include "decomp.h"
#include "sead/seadDelegateBase.h"
#include "sead/seadIDelegate2.h"
#include "sead/seadNew.h"

namespace sead {
// Instantiations found in the binary:
//   sead::Delegate2<BossSys, sead::Thread*, int>  typeinfo 0x008D22FC  vtable 0x00906FB8
//   sead::Delegate2<net::nex::Framework, sead::Thread*, int>  typeinfo 0x008D2308  vtable 0x00906FC8
//   sead::Delegate2<sead::TaskMgr, sead::Thread*, int>  typeinfo 0x008D2320  vtable 0x00906FE8
//   sead::Delegate2<ugc::Text, sead::Thread*, int>  typeinfo 0x008D2314  vtable 0x00906FD8
//
// Calls a member function of an object; clone copies the delegate onto a heap.
template <typename T0, typename T1, typename T2>
class Delegate2 : public DelegateBase<T0, void (T0::*)(T1, T2), IDelegate2<T1, T2>>
{
public:
    typedef DelegateBase<T0, void (T0::*)(T1, T2), IDelegate2<T1, T2>> Base;

    Delegate2() : Base() {}
    Delegate2(T0* object, typename Base::MethodType method) : Base(object, method) {}

    virtual void invoke(T1 arg1, T2 arg2)
    {
        if (this->mObject != nullptr && this->mMethod != nullptr) {
            (this->mObject->*this->mMethod)(arg1, arg2);
        }
    }
    virtual IDelegate2<T1, T2>* clone(Heap* heap) const { return new (heap, 4) Delegate2(*this); }
};
} // namespace sead
