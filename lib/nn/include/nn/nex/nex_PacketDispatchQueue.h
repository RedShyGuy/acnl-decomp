#pragma once

#include "decomp.h"
#include "nn/nex/nex_LogicalClockTmpl.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class PacketIn;

// The received reliable packets of a substream that wait for their turn: they are dispatched in
// the order of their sequence ids. The names of the members are ours.
class PacketDispatchQueue : public ::nn::nex::RootObject
{
public:
    typedef LogicalClockTmpl<u16, s16> SequenceId;
    typedef std::map<SequenceId, PacketIn*> PacketMap;

    PacketDispatchQueue(); // 0x00394534 | mk7dlp:bytes [tier A]
    // (inline)
    ~PacketDispatchQueue() { Purge(); }

    // the packet with the next sequence id (null if it was not received yet)
    nn::nex::PacketIn* GetNextToDispatch(); // 0x003941B8 | fefates:bytes [tier B]
    // removes all packets
    void Purge(); // 0x00394244 | fefates:bytes [tier B]
    // queues a packet that is not older than the next one (once)
    void Queue(nn::nex::PacketIn* pPacket); // 0x00394324 | fefates:bytes [tier B]

    u32 m_Unknown0x00;            // 0x00 (not set by the constructor)
    PacketMap m_Packets;          // 0x04
    SequenceId m_NextSequenceId;  // 0x20
};
ASSERT_OFFSET(PacketDispatchQueue, m_NextSequenceId, 0x20);
ASSERT_SIZE(PacketDispatchQueue, 0x28);
} // namespace nex
} // namespace nn
