#pragma once

#include "decomp.h"
#include "nn/nex/nex_Socket.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex13QueuingSocketE @ 0x008CE260
// vtable 0x008FC8D4 (vptr 0x008FC8DC), offset_to_top 0, 9 entries
class QueuingSocket : public ::nn::nex::Socket
{
public:
    QueuingSocket(); // ctor candidate(s) 0x0036A8F0 (unverified)
    virtual ~QueuingSocket(); // 0x0036AA1C slot 0x00 | fefates:callseq
    // 0x0036A9EC slot 0x04 | slot vf_0x04 of nn::nex::Socket (deleting dtor)
    virtual void vf_0x08(); // 0x0036A7BC slot 0x08 | virtual slot, introduced by nn::nex::Socket
    virtual void vf_0x0C(); // 0x0036A840 slot 0x0C | virtual slot, introduced by nn::nex::Socket
    virtual void vf_0x10(); // 0x00369624 slot 0x10 | virtual slot, introduced by nn::nex::Socket
    virtual void vf_0x14(); // 0x0036A848 slot 0x14 | virtual slot, introduced by nn::nex::Socket
    virtual void vf_0x18(); // 0x0072A994 slot 0x18 | virtual slot, introduced by nn::nex::QueuingSocket
    virtual void vf_0x1C(); // 0x00369594 slot 0x1C | virtual slot, introduced by nn::nex::QueuingSocket
    virtual void vf_0x20(); // 0x00369544 slot 0x20 | virtual slot, introduced by nn::nex::QueuingSocket
    void RetransmitFromHistoryPacketQueue(nn::nex::PacketQueue*, nn::nex::qChain<nn::nex::Packet*,nn::nex::ChainPolicyHistoryPacket<nn::nex::Packet*>>*, unsigned short); // 0x0036A628 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
