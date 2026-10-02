#pragma once

#include "decomp.h"
#include "nn/nex/nex_NATTraversalRelayProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29NATTraversalRelayProtocolImplE @ 0x008CF148
// vtable 0x008FF2B0 (vptr 0x008FF2B8), offset_to_top 0, 31 entries
class NATTraversalRelayProtocolImpl : public ::nn::nex::NATTraversalRelayProtocol
{
public:
    NATTraversalRelayProtocolImpl(); // ctor address unknown
    virtual ~NATTraversalRelayProtocolImpl(); // 0x003C1254 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C1200 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void InitiateProbe(const nn::nex::StationURL&); // 0x003C11E0 slot 0x60 | fefates:bytes
};
} // namespace nex
} // namespace nn
