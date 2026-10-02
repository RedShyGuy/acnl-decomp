#pragma once

#include "decomp.h"
#include "nn/nex/nex_Operation.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27CallProtocolMethodOperationE @ 0x008CEFE8
// vtable 0x008FEEB0 (vptr 0x008FEEB8), offset_to_top 0, 6 entries
class CallProtocolMethodOperation : public ::nn::nex::Operation
{
public:
    CallProtocolMethodOperation(); // ctor address unknown
    virtual void vf_0x00(); // 0x003BC2F8 slot 0x00 | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x04(); // 0x003BC2E8 slot 0x04 | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x0C(); // 0x0072DE18 slot 0x0C | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x10(); // 0x0072DDE8 slot 0x10 | virtual slot, introduced by nn::nex::Operation
    virtual void vf_0x14(); // 0x003BC2E4 slot 0x14 | virtual slot, introduced by nn::nex::Operation
    void InitializeForNewCallAttempt(); // 0x003BC298 | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
