#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex7TimeoutE @ 0x008CF68C
// vtable 0x008FFB78 (vptr 0x008FFB80), offset_to_top 0, 2 entries
class Timeout : public ::nn::nex::RootObject
{
public:
    Timeout(); // ctor address unknown
    virtual void vf_0x00(); // 0x003D3814 slot 0x00 | virtual slot, introduced by nn::nex::Timeout
    virtual void vf_0x04(); // 0x003D37F0 slot 0x04 | virtual slot, introduced by nn::nex::Timeout
    void SetRelativeExpirationTime(int); // 0x003D3794 | mk7dlp:bytes [tier A]
    void IsExpired() const; // 0x0072E478 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
