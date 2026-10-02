#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex5RelayE @ 0x008CF544
// vtable 0x008FF9EC (vptr 0x008FF9F4), offset_to_top 0, 2 entries
class Relay : public ::nn::nex::RootObject
{
public:
    Relay(); // ctor address unknown
    virtual void vf_0x00(); // 0x003CF1E4 slot 0x00 | virtual slot, introduced by nn::nex::Relay
    virtual void vf_0x04(); // 0x003CF154 slot 0x04 | virtual slot, introduced by nn::nex::Relay
    void ShouldRelay(const nn::nex::InetAddress*); // 0x003CEDB4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
