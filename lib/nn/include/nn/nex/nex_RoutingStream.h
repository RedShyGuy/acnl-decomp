#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RoutingTable.h"
#include "nn/nex/nex_Stream.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qMap.h"

#include <utility>

namespace nn {
namespace nex {
class Buffer;
class PacketOut;
class RootTransport;

// The stream of the routing (port 1 of the transport with the router enabled): data for this
// station goes to the transport, data for others is forwarded. The names of the members and of the
// functions without a reference name are ours.
// RTTI N2nn3nex13RoutingStreamE @ 0x008CE284
// vtable 0x008FC974 (vptr 0x008FC97C), offset_to_top 0, 5 entries
class RoutingStream : public ::nn::nex::Stream
{
public:
    // the packet types of the routing
    static const u32 PACKET_TYPE_DATA = 2;
    static const u32 PACKET_TYPE_FORWARD = 6;
    // decides whether a packet from the source is forwarded to the destination
    typedef bool (*ForwardFilter)(const InetAddress* pSource, const InetAddress* pDestination, Packet* pPacket);
    typedef std::pair<InetAddress, InetAddress> Route;

    RoutingStream(); // 0x0036D064 | fefates:bytes-fuzzy [tier B]
    virtual ~RoutingStream(); // 0x0036D22C slot 0x00 | fefates:callseq
    // 0x0036D1FC (deleting dtor)
    virtual bool ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket); // 0x0036C2DC slot 0x08 | fefates:callseq
    virtual void DoWork(); // 0x0036D004 slot 0x0C

    // a packet of the type with the destination and the payload (from the pools if the payload is)
    static nn::nex::PacketOut* CreatePacket(nn::nex::Buffer* pPayload, unsigned int type, const nn::nex::InetAddress* pDestination); // 0x0036C0AC (name is ours)
    // the forwarded routes that were not used for s_RouteTimeout are removed
    void RemoveExpiredRoutes(); // 0x0036CDC0 (name is ours)
    // the number of the forwarded routes (to the station if not null)
    u32 GetRouteCount(const nn::nex::InetAddress* pAddress); // 0x0036BFB0 (name is ours)

    // the period of RemoveExpiredRoutes (0: the routes are not counted)
    static u32 s_CleanupPeriod;
    // the time of a forwarded route
    static u32 s_RouteTimeout;
    // the maximum number of forwarded routes (0: no maximum)
    static u32 s_MaxRoutes;

    qMap<Route, Time> m_Routes;       // 0x08 (the last use of the forwarded routes)
    qMap<InetAddress, Time> m_Peers;  // 0x24
    Time m_LastCleanup;               // 0x40
    u32 m_Unknown0x48;                // 0x48
    CriticalSection m_Lock;           // 0x4C
    RoutingTable m_RoutingTable;      // 0x78
    RootTransport* m_pTransport;      // 0xA8
    ForwardFilter m_pFilter;          // 0xAC
    u8 m_Unknown0xB0[0x8];            // 0xB0
};
ASSERT_OFFSET(RoutingStream, m_Lock, 0x4C);
ASSERT_OFFSET(RoutingStream, m_RoutingTable, 0x78);
ASSERT_OFFSET(RoutingStream, m_pTransport, 0xA8);
ASSERT_SIZE(RoutingStream, 0xB8);
} // namespace nex
} // namespace nn

namespace __rw {
// (the range erases of the maps of the routing stream are out of line; nex_RoutingStream.cpp)
template <>
DECOMP_NOINLINE __rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>,
                          __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>,
                          nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::iterator
__rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>,
          __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>,
          nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::erase(iterator first, iterator last);
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::Time>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::Time>, nn::nex::InetAddress>,
                          std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::Time> > >::iterator
__rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::Time>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::Time>, nn::nex::InetAddress>,
          std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::Time> > >::erase(iterator first, iterator last);
} // namespace __rw
