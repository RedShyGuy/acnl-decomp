#pragma once

#include "decomp.h"
#include "nn/nex/nex_NATTraversalRelayProtocol.h"

namespace nn {
namespace nex {
// The NAT traversal relay protocol that passes the probe requests of the server to the relay
// interface of its relay client.
// RTTI N2nn3nex29NATTraversalRelayProtocolImplE @ 0x008CF148
// vtable 0x008FF2B0 (vptr 0x008FF2B8), offset_to_top 0, 31 entries
class NATTraversalRelayProtocolImpl : public ::nn::nex::NATTraversalRelayProtocol
{
public:
    // (inline in NATTraversalRelayClient::CreateNATTraversalRelayProtocol)
    NATTraversalRelayProtocolImpl() {}
    virtual ~NATTraversalRelayProtocolImpl(); // 0x003C1254 slot 0x00
    // 0x003C1200 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void InitiateProbe(const nn::nex::StationURL& url); // 0x003C11E0 slot 0x60 | fefates:bytes
};
ASSERT_SIZE(NATTraversalRelayProtocolImpl, 0x90);
} // namespace nex
} // namespace nn
