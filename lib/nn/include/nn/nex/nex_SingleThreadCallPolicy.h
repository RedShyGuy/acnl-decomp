#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallPolicy.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22SingleThreadCallPolicyE @ 0x008CEC68
// vtable 0x008FE1C0 (vptr 0x008FE1C8), offset_to_top 0, 4 entries
class SingleThreadCallPolicy : public ::nn::nex::CallPolicy
{
public:
    virtual void vf_0x00(); // 0x0039E2A4 slot 0x00 | virtual slot, introduced by nn::nex::SingleThreadCallPolicy
    virtual void vf_0x04(); // 0x0039E28C slot 0x04 | virtual slot, introduced by nn::nex::SingleThreadCallPolicy
    virtual void vf_0x08(); // 0x0039E21C slot 0x08 | virtual slot, introduced by nn::nex::SingleThreadCallPolicy
    virtual void EndCallImpl(); // 0x0039E200 slot 0x0C | mk7dlp:bytes
    SingleThreadCallPolicy(); // 0x0039E264 | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
