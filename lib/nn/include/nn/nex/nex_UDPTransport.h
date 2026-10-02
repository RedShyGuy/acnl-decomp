#pragma once

#include "decomp.h"
#include "nn/nex/nex_SocketTransport.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12UDPTransportE @ 0x008CE1DC
// vtable 0x008FC728 (vptr 0x008FC730), offset_to_top 0, 17 entries
class UDPTransport : public ::nn::nex::SocketTransport
{
public:
    UDPTransport(); // ctor address unknown
    virtual ~UDPTransport(); // 0x0037D530 slot 0x00 | slot vf_0x00 of nn::nex::RootTransport
    // 0x00361654 slot 0x04 | slot vf_0x04 of nn::nex::RootTransport (deleting dtor)
};
} // namespace nex
} // namespace nn
