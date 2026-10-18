#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// The routes of the routing and relay streams: the address through which a destination is reached.
// An entry expires when it was not refreshed for its time (the names of the members and of the
// functions without a reference name are ours).
class RoutingTable
{
public:
    // a route (the class name is from the binary)
    struct RTEntry
    {
        RTEntry(const InetAddress& via, u32 timeLeft) : m_Via(via), m_TimeLeft(timeLeft) {}

        InetAddress m_Via; // 0x00
        u32 m_TimeLeft;    // 0x20 (ms)
    };
    typedef qMap<InetAddress, RTEntry> Map;

    // timeout: the time of an entry (ms)
    explicit RoutingTable(unsigned int timeout); // 0x00360828 | fefates:bytes [tier B]
    // the route to the destination through via (overwrite: also if there is one); false for an
    // invalid one
    bool Add(const nn::nex::InetAddress& destination, const nn::nex::InetAddress& via, bool overwrite); // 0x003604AC | fefates:bytes [tier B]
    bool Remove(const nn::nex::InetAddress& destination); // 0x003606EC | fefates:bytes [tier B]
    // pVia: the address through which the destination is reached (its time starts again)
    bool Find(const nn::nex::InetAddress& destination, nn::nex::InetAddress& via) const; // 0x0072A538 | fefates:bytes [tier B]
    // (once a second) the time of the entries goes down, the expired ones are removed
    void Update(); // 0x00360384 (name is ours)

    u32 m_Unknown0x00;    // 0x00 (not initialized)
    Map m_Entries;        // 0x04
    u32 m_Timeout;        // 0x20
    Time m_LastUpdate;    // 0x28
};
ASSERT_OFFSET(RoutingTable, m_Timeout, 0x20);
ASSERT_SIZE(RoutingTable, 0x30);
} // namespace nex
} // namespace nn

namespace __rw {
// (the range erase of the routing table's tree is out of line; nex_RoutingTable.cpp)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>,
                          __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
                          nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::iterator
__rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>,
          __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
          nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::erase(iterator first, iterator last);
} // namespace __rw
