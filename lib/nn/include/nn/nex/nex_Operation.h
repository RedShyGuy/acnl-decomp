#pragma once

#include "decomp.h"
#include "nn/nex/nex_StateMachine.h"
#include "nn/nex/nex_StateMachine_QEvent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9OperationE @ 0x008CF774
// vtable 0x008FFD5C (vptr 0x008FFD64), offset_to_top 0, 6 entries
class Operation : public ::nn::nex::StateMachine::QEvent
{
public:
    Operation(); // ctor candidate(s) 0x003D753C (unverified)
    virtual void vf_0x00(); // 0x003D7570 slot 0x00 | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x04(); // 0x003D7560 slot 0x04 | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x08(); // 0x0072EA80 slot 0x08 | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
};
} // namespace nex
} // namespace nn
