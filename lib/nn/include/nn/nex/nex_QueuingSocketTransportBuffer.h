#pragma once

#include "decomp.h"
#include "nn/nex/nex_QueuingSocket.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex28QueuingSocketTransportBufferE @ 0x008CF0B4
// vtable 0x008FF098 (vptr 0x008FF0A0), offset_to_top 0, 9 entries
class QueuingSocketTransportBuffer : public ::nn::nex::QueuingSocket
{
public:
    QueuingSocketTransportBuffer(); // ctor address unknown
    virtual ~QueuingSocketTransportBuffer(); // 0x003BE368 slot 0x00 | slot vf_0x00 of nn::nex::Socket
    // 0x003BE2E8 slot 0x04 | slot vf_0x04 of nn::nex::Socket (deleting dtor)
    virtual void vf_0x18(); // 0x0072DE44 slot 0x18 | virtual slot, introduced by nn::nex::QueuingSocket
    virtual void vf_0x1C(); // 0x003BDF00 slot 0x1C | virtual slot, introduced by nn::nex::QueuingSocket
    virtual void vf_0x20(); // 0x003BDDFC slot 0x20 | virtual slot, introduced by nn::nex::QueuingSocket
};
} // namespace nex
} // namespace nn
