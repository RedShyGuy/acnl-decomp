#include "nn/nex/nex_RoutingTable.h"

namespace nn {
namespace nex {
namespace {
// the period of Update (ms)
const u32 UPDATE_PERIOD = 1000;
} // namespace

// 0x00360384 (name is ours)
void nn::nex::RoutingTable::Update()
{
    Time now = SystemClock::GetTime();
    u32 elapsed = static_cast<u32>(now.m_Value - m_LastUpdate.m_Value);
    if (elapsed < UPDATE_PERIOD) {
        return;
    }
    m_LastUpdate = now;
    for (Map::iterator it = m_Entries.begin(); it != m_Entries.end();) {
        if (it->second.m_TimeLeft > elapsed) {
            it->second.m_TimeLeft -= elapsed;
            ++it;
        } else {
            m_Entries.erase(it++);
        }
    }
}

// 0x003604AC | fefates:bytes [tier B]
bool nn::nex::RoutingTable::Add(const nn::nex::InetAddress& destination, const nn::nex::InetAddress& via, bool overwrite)
{
    if (via.m_SockAddr.m_Address == 0) {
        return false;
    }
    if (destination.IsSame(via)) {
        return false;
    }
    std::pair<Map::iterator, bool> result = m_Entries.m_Tree.InsertInline(Map::value_type(destination, RTEntry(via, m_Timeout)));
    if (result.second) {
        return true;
    }
    if (!overwrite) {
        return false;
    }
    result.first->second.m_TimeLeft = m_Timeout;
    result.first->second.m_Via = via;
    return true;
}

// 0x003606EC | fefates:bytes [tier B]
bool nn::nex::RoutingTable::Remove(const nn::nex::InetAddress& destination)
{
    return m_Entries.EraseInline(destination) == 1;
}

// 0x00360828 | fefates:bytes [tier B]
nn::nex::RoutingTable::RoutingTable(unsigned int timeout) : m_Timeout(timeout), m_LastUpdate(SystemClock::GetTime())
{
    // nothing to do
}

// 0x0072A538 | fefates:bytes [tier B]
bool nn::nex::RoutingTable::Find(const nn::nex::InetAddress& destination, nn::nex::InetAddress& via) const
{
    Map::iterator it = m_Entries.find(destination);
    if (it == m_Entries.end()) {
        return false;
    }
    it->second.m_TimeLeft = m_Timeout;
    via = it->second.m_Via;
    return true;
}

} // namespace nex
} // namespace nn

namespace __rw {
// the functions of the tree of the routing table
// 0x007F7F00 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>,
                          __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
                          nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::iterator
__rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>,
          __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
          nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x007F78A4 (name is ours)
template __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::iterator __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::erase(iterator it);
// 0x007F8040 (name is ours)
template void __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::_C_erase(Node* pNode);
// 0x007F807C | fefates:bytes [tier B]
template __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::iterator __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const nn::nex::InetAddress, nn::nex::RoutingTable::RTEntry>& value);
} // namespace __rw
