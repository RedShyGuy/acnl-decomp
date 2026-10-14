#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_LogicalClockTmpl.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class PacketOut;

// The packets of a reliable substream that are sent and not yet acknowledged, by sequence id. At
// most m_WindowSize of them are sent before the oldest is acknowledged, at most m_MaxPackets are
// queued. The functions run under the lock of the scheduler. The names of the members and of the
// functions but the constructor are ours.
class SlidingWindow : public ::nn::nex::RootObject
{
public:
    typedef LogicalClockTmpl<u16, s16> SequenceId;
    typedef std::map<SequenceId, PacketOut*> PacketMap;

    SlidingWindow(unsigned short windowSize, unsigned int maxPackets); // 0x0036E1AC | fefates:bytes [tier B]
    ~SlidingWindow(); // 0x0036E298 (name is ours)

    // removes the acknowledged packet; its id in pIdCounts loses a reference (the ids without
    // references go to Network::m_ReleasedIds)
    void Acknowledge(const nn::nex::LogicalClockTmpl<unsigned short, short>& sequenceId, nn::nex::qMap<unsigned int, unsigned int>* pIdCounts); // 0x0036D8D8 (name is ours)
    // the next packet to send in the window (null when the window is full or there is none)
    nn::nex::PacketOut* GetNextPacketToSend(); // 0x0036DC5C (name is ours)
    // queues the packet with the next sequence id (false when the window has no room)
    bool AddPacket(nn::nex::PacketOut* pPacket); // 0x0036DD2C (name is ours)
    // removes all packets
    void Purge(); // 0x0036DFB0 (name is ours)
    // the packet of the sequence id (null if there is none)
    nn::nex::PacketOut* GetPacket(const nn::nex::LogicalClockTmpl<unsigned short, short>& sequenceId); // 0x0036E0E0 (name is ours)

    u32 m_Unknown0x00;            // 0x00 (not set by the constructor)
    PacketMap m_Packets;          // 0x04
    u16 m_WindowSize;             // 0x20
    SequenceId m_NextSequenceId;  // 0x24 (of the next added packet)
    SequenceId m_NextToSend;      // 0x2C
    SequenceId m_FirstUnacked;    // 0x34
    s32 m_DataPacketCount;        // 0x3C (the packets of type DATA)
    u32 m_MaxPackets;             // 0x40
    CriticalSection m_Lock;       // 0x44
};
ASSERT_OFFSET(SlidingWindow, m_WindowSize, 0x20);
ASSERT_OFFSET(SlidingWindow, m_Lock, 0x44);
ASSERT_SIZE(SlidingWindow, 0x6C);
} // namespace nex
} // namespace nn
