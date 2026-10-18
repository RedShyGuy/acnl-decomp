#pragma once

#include "decomp.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class PacketOut;
} // namespace nex
} // namespace nn

namespace __rw {
// (the original calls erase(first, last) of the scheduled packets out of line; defined in
// nex_TimeoutManager.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::PacketOut*, nn::nex::PacketOut*, __ident<nn::nex::PacketOut*, nn::nex::PacketOut*>, std::less<nn::nex::PacketOut*>,
                          nn::nex::MemAllocator<nn::nex::PacketOut*> >::iterator
__rb_tree<nn::nex::PacketOut*, nn::nex::PacketOut*, __ident<nn::nex::PacketOut*, nn::nex::PacketOut*>, std::less<nn::nex::PacketOut*>,
          nn::nex::MemAllocator<nn::nex::PacketOut*> >::erase(iterator first, iterator last);
} // namespace __rw

namespace nn {
namespace nex {
// The timeouts of the packets of a PRUDPStream: the scheduled packets and their resend times.
// The names of the members and of the functions without a reference name are ours.
class TimeoutManager
{
public:
    TimeoutManager(); // 0x00375828 | fefates:bytes [tier B]
    ~TimeoutManager(); // 0x0037594C | fefates:bytes [tier B]

    // the packet is resent when its timeout expires
    void SchedulePacketTimeout(nn::nex::PacketOut* pPacket); // 0x00374CBC
    // the packet is not resent any more; false if it was not scheduled
    bool UnschedulePacketTimeout(nn::nex::PacketOut* pPacket); // 0x00374A10 (name is ours)
    // resends the packets whose timeout expired
    void ProcessTimeouts(); // 0x00375244 (name is ours)
    // no timeout expires after the deadline
    void LimitExpirationTimes(const nn::nex::Time& deadline); // 0x00375728 (name is ours)

    u32 m_Unknown0x00;                               // 0x00 (not set by the constructor)
    std::set<PacketOut*> m_Packets;                  // 0x04 (the scheduled packets)
    std::multimap<Time, PacketOut*> m_Timeouts;      // 0x20 (the packets by resend time)
    bool m_Unknown0x3C;                              // 0x3C (the timeouts of the connection packets expire at once)
};
ASSERT_OFFSET(TimeoutManager, m_Timeouts, 0x20);
ASSERT_SIZE(TimeoutManager, 0x40);
} // namespace nex
} // namespace nn
