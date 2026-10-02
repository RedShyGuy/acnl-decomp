#pragma once

#include "decomp.h"

namespace pead {
// Instantiations found in the binary:
//   pead::DelegateBase<nn::pia::common::BackgroundScheduler, void (nn::pia::common::BackgroundScheduler::*)(pead::Thread*, int), pead::IDelegate2<pead::Thread*, int> >  typeinfo 0x008D1150
//   pead::DelegateBase<nn::pia::transport::TransportThreadStream, void (nn::pia::transport::TransportThreadStream::*)(pead::Thread*, int), pead::IDelegate2<pead::Thread*, int> >  typeinfo 0x008D115C
template <typename T0, typename T1, typename T2>
class DelegateBase
{
public:
    // TODO: members unknown
};
} // namespace pead
