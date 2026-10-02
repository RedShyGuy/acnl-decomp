#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16ProtocolRegistryE @ 0x008CE5FC
// vtable 0x008FD214 (vptr 0x008FD21C), offset_to_top 0, 2 entries
class ProtocolRegistry : public ::nn::nex::RootObject
{
public:
    ProtocolRegistry(); // ctor address unknown
    virtual ~ProtocolRegistry(); // 0x003837C0 slot 0x00 | fefates:bytes
    // 0x003837B0 slot 0x04 | slot vf_0x04 of nn::nex::ProtocolRegistry (deleting dtor)
    void RegisterProtocol(unsigned short, nn::nex::Protocol*); // 0x0038378C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
