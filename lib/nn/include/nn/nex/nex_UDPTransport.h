#pragma once

#include "decomp.h"
#include "nn/nex/nex_SocketTransport.h"

namespace nn {
namespace nex {
// The socket transport over UDP (the transport of the network).
// RTTI N2nn3nex12UDPTransportE @ 0x008CE1DC
// vtable 0x008FC728 (vptr 0x008FC730), offset_to_top 0, 17 entries
class UDPTransport : public ::nn::nex::SocketTransport
{
public:
    // (inline in the constructor of Network)
    UDPTransport() {}
    virtual ~UDPTransport(); // 0x0037D530 slot 0x00
    // 0x00361654 slot 0x04 (deleting dtor)
};
} // namespace nex
} // namespace nn
