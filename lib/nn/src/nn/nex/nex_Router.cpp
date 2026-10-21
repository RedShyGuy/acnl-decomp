#include "nn/nex/nex_Router.h"
#include "nn/nex/nex_PacketOut.h"

namespace nn {
namespace nex {
// 0x003D05FC | fefates:bytes-fuzzy [tier B]
bool nn::nex::Router::ShouldRoute(const nn::nex::InetAddress* pAddress)
{
    if (!m_IsEnabled) {
        return false;
    }
    InetAddress via;
    if (!m_pStream->m_RoutingTable.Find(*pAddress, via)) {
        return false;
    }
    return !pAddress->IsSame(via);
}

// 0x003D069C (name is ours)
bool nn::nex::Router::Route(nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress, unsigned short port)
{
    InetAddress via;
    m_pStream->m_RoutingTable.Find(*pAddress, via);
    PacketOut* pPacket = RoutingStream::CreatePacket(pData, RoutingStream::PACKET_TYPE_FORWARD, pAddress);
    if (pPacket == nullptr) {
        return false;
    }
    if ((pPacket->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
        delete pPacket;
        return false;
    }
    pPacket->m_Address.m_SockAddr = via.m_SockAddr;
    bool result = m_pStream->m_pTransport->SendPacket(port, m_pStream->m_Type, PORT, PORT, pPacket, true);
    pPacket->ReleaseRef();
    return result;
}

// 0x003D07C8 | fefates:bytes-fuzzy [tier B]
nn::nex::Router::Router() : m_IsEnabled(false), m_IsRegistered(false)
{
    m_pPortMap = new PortMap();
    m_pStream = new RoutingStream();
}

// 0x003D0988 slot 0x00 | fefates:bytes-fuzzy
// 0x003D0958 (deleting dtor)
nn::nex::Router::~Router()
{
    if (m_IsRegistered && m_pStream->m_pTransport != nullptr) {
        m_IsRegistered = !m_pStream->m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_pStream->m_Type), PORT);
    }
    delete m_pPortMap;
    delete m_pStream;
}

} // namespace nex
} // namespace nn

namespace __rw {
// the range erases of the port maps
// 0x0080B3AC (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>,
                          nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::iterator
__rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>,
          nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x007F9284 (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>,
                          std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::iterator
__rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
          nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x0080B4EC (name is ours)
template void __rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::_C_erase(Node* pNode);
// 0x007F93C4 (name is ours)
template void __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::_C_erase(Node* pNode);
// 0x0080AD50 (name is ours)
template __rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::iterator __rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::erase(iterator it);
// 0x007F8C28 (name is ours)
template __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::iterator __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::erase(iterator it);
} // namespace __rw
