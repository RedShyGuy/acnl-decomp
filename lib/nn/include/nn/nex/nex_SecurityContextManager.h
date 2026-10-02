#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22SecurityContextManagerE @ 0x008CEC5C
// vtable 0x008FE1B0 (vptr 0x008FE1B8), offset_to_top 0, 2 entries
class SecurityContextManager : public ::nn::nex::RootObject
{
public:
    SecurityContextManager(); // ctor address unknown
    virtual void vf_0x00(); // 0x0039E1E4 slot 0x00 | virtual slot, introduced by nn::nex::SecurityContextManager
    virtual void vf_0x04(); // 0x0039E1B0 slot 0x04 | virtual slot, introduced by nn::nex::SecurityContextManager
    void StaticGetCurrentCID(); // 0x0039DE34 | fefates:bytes [tier B]
    void StaticGetCurrentPID(); // 0x0039DE7C | fefates:bytes [tier B]
    void Pop(); // 0x0039DEC4 | mk7dlp:bytes [tier A]
    void Push(unsigned, unsigned); // 0x0039DF20 | mk7dlp:callseq [tier A]
    void GetCurrentAddress() const; // 0x0072D0B0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
