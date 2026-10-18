#pragma once

#include "decomp.h"
#include "nn/nex/nex_qVector.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_Timeout.h"

namespace nn {
namespace nex {
class PRUDPEndPoint;

// A packet to send: its end point, the timeout of an acknowledged packet and the state of the
// sending. The member names are ours.
// RTTI N2nn3nex9PacketOutE @ 0x008CF780
// vtable 0x008FFD7C (vptr 0x008FFD84), offset_to_top 0, 2 entries
class PacketOut : public ::nn::nex::Packet
{
public:
    // the flag of the packets that need an acknowledgement (they get a timeout; the name is ours)
    static const u16 FLAG_NEED_ACK = 0x40;

    PacketOut(nn::nex::PRUDPEndPoint* pEndPoint, nn::nex::PacketType type, unsigned short flags, nn::nex::Buffer* pPayload, unsigned int value); // 0x003D7780 | fefates:bytes [tier B]
    virtual ~PacketOut(); // 0x003D7884 slot 0x00 | fefates:bytes
    // 0x003D7850 slot 0x04 (deleting dtor)
    // (a new payload replaces the old one; a null payload keeps it)
    void Initialize(nn::nex::PRUDPEndPoint* pEndPoint, nn::nex::PacketType type, unsigned short flags, nn::nex::Buffer* pPayload, unsigned int value, nn::nex::Timeout* pTimeout); // 0x003D7580 | fefates:bytes [tier B]
    // whether the packet may be sent now: always without bundling, else once the bundles of its
    // stream type were sent after the packet was queued
    bool IsBundlingTimeReached(); // 0x003D7670 (name is ours)

    Timeout* m_pTimeout;           // 0x118
    PRUDPEndPoint* m_pEndPoint;    // 0x11C
    u16 m_Unknown0x120;            // 0x120
    u16 m_Unknown0x122;            // 0x122
    u32 m_Unknown0x124;            // 0x124
    u32 m_Unknown0x128;            // 0x128
    u32 m_Unknown0x12C;            // 0x12C
    u8 m_Unknown0x130;             // 0x130
    u8 m_Unknown0x131;             // 0x131
    u32 m_Unknown0x134;            // 0x134
    u8 m_Unknown0x138;             // 0x138
    Time m_Unknown0x140;           // 0x140
    u32 m_Unknown0x148;            // 0x148 (the value of the constructor)

protected:
    // (for the derived packets of the PacketBufferManager)
    PacketOut() : m_Unknown0x128(0), m_Unknown0x12C(0) {}
};
ASSERT_OFFSET(PacketOut, m_Unknown0x140, 0x140);
ASSERT_SIZE(PacketOut, 0x150);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::PacketOut*, nn::nex::MemAllocator<nn::nex::PacketOut*> >::_C_insert_aux(nn::nex::PacketOut** pos, nn::nex::PacketOut* const& value);
} // namespace std
