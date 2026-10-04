#pragma once

#include "decomp.h"
#include "pead/peadDelegateBase.h"
#include "pead/peadIDelegate2.h"

namespace pead {
// Instantiations found in the binary:
//   pead::Delegate2<nn::pia::common::BackgroundScheduler, pead::Thread*, int>  typeinfo 0x008D130C  vtable 0x00904DAC
//   pead::Delegate2<nn::pia::transport::TransportThreadStream, pead::Thread*, int>  typeinfo 0x008D1318  vtable 0x00904DB8
//
// Calls a member function of an object with two arguments.
template <typename T0, typename T1, typename T2>
class Delegate2 : public DelegateBase<T0, void (T0::*)(T1, T2), IDelegate2<T1, T2> >
{
public:
    typedef DelegateBase<T0, void (T0::*)(T1, T2), IDelegate2<T1, T2> > Base;

    Delegate2(T0* object, void (T0::*method)(T1, T2)) : Base(object, method) {}

    // <BackgroundScheduler, Thread*, int>: 0x008100C8 slot 0x00
    virtual void invoke(T1 arg1, T2 arg2)
    {
        if (this->mObject && this->mMethod) {
            (this->mObject->*this->mMethod)(arg1, arg2);
        }
    }
};
} // namespace pead
