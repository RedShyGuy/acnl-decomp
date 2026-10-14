#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Packet.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
// A queue of packets (a chain over the links of the packets, Packet +0x0C/+0x10). The queue holds
// a reference to each packet. The member names are ours.
// RTTI N2nn3nex11PacketQueueE @ 0x008CE05C
// vtable 0x008FC3C4 (vptr 0x008FC3CC), offset_to_top 0, 7 entries
class PacketQueue : public ::nn::nex::RootObject
{
public:
    typedef qChain<Packet*, DefaultChainPolicy<Packet*> > PacketChain;

    // (inline)
    PacketQueue() {}
    virtual ~PacketQueue(); // 0x0035B610 slot 0x00
    // 0x0035B5D0 slot 0x04 (deleting dtor)
    // (noinline: the destructor calls it)
    DECOMP_NOINLINE virtual void Purge(); // 0x0035B408 slot 0x08 | mk7dlp:bytes
    // appends a packet that is in no queue (setTime: the time of the queuing becomes the time of
    // the packet)
    virtual void Queue(nn::nex::Packet* pPacket, bool setTime); // 0x0035B454 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
    virtual void QueueFront(nn::nex::Packet* pPacket, bool setTime); // 0x0035B344 slot 0x10 | fefates:bytes
    // removes the packet; the next one
    virtual nn::nex::PacketQueue::PacketChain::iterator Dequeue(nn::nex::PacketQueue::PacketChain::iterator it); // 0x0035B514 slot 0x14 | fefates:bytes
    // the lock of the queue (none)
    virtual nn::nex::CriticalSection* GetLock(); // 0x0035B5C8 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue

    PacketChain m_Packets; // 0x04
};
ASSERT_SIZE(PacketQueue, 0x18);
} // namespace nex
} // namespace nn
