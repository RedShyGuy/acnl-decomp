#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::DelegateBase<nn::pia::common::BackgroundScheduler, void (nn::pia::common::BackgroundScheduler::*)(pead::Thread*, int), pead::IDelegate2<pead::Thread*, int> >  typeinfo 0x008D1150
//   pead::DelegateBase<nn::pia::transport::TransportThreadStream, void (nn::pia::transport::TransportThreadStream::*)(pead::Thread*, int), pead::IDelegate2<pead::Thread*, int> >  typeinfo 0x008D115C
//
// An object and a member function of it. Layout from pia's BackgroundScheduler constructor
// (vptr, object, member function pointer); the member names are ours.
template <typename T0, typename T1, typename T2>
class DelegateBase : public T2
{
public:
    DelegateBase(T0* object, T1 method) : mObject(object), mMethod(method) {}

    T0* mObject; // 0x04
    T1 mMethod;  // 0x08
};
} // namespace pead
