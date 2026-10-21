#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_RoutingStream.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class Buffer;

// The routing of a transport (SocketTransport +0x204, RootTransport::GetRouter): with it enabled
// the routing stream listens on port 1 and the packets for other stations go through the routing
// table. The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex6RouterE @ 0x008CF5B8
// vtable 0x008FFA2C (vptr 0x008FFA34), offset_to_top 0, 2 entries
class Router : public ::nn::nex::RootObject
{
public:
    // the port of the routing stream
    static const u8 PORT = 1;

    // the ports of the routed stations (both directions)
    // (value-initialized with new)
    struct PortMap : public RootObject
    {
        u32 m_Unknown0x00;                  // 0x00
        qMap<u16, InetAddress> m_Addresses; // 0x04
        qMap<InetAddress, u16> m_Ports;     // 0x20
    };

    Router(); // 0x003D07C8 | fefates:bytes-fuzzy [tier B]
    virtual ~Router(); // 0x003D0988 slot 0x00 | fefates:bytes-fuzzy
    // 0x003D0958 (deleting dtor)
    // true if the packets to the address go through another station
    bool ShouldRoute(const nn::nex::InetAddress* pAddress); // 0x003D05FC | fefates:bytes-fuzzy [tier B]
    // sends the packet (with the routing header) to the station through which the address is reached
    bool Route(nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress, unsigned short port); // 0x003D069C (name is ours)

    // (inline) the routing stream listens on its port (or no longer)
    void Enable(bool enable)
    {
        m_IsEnabled = enable;
        if (enable) {
            if (m_pStream->m_pTransport != nullptr) {
                m_IsRegistered = m_pStream->m_pTransport->m_StreamManager.RegisterStream(m_pStream, PORT, nullptr);
            }
        } else if (m_IsRegistered) {
            if (m_pStream->m_pTransport != nullptr) {
                m_IsRegistered = !m_pStream->m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_pStream->m_Type), PORT);
            }
        }
    }

    // (inline in the constructor of SocketTransport) the routing stream belongs to the transport
    void SetTransport(RootTransport* pTransport)
    {
        if (m_IsRegistered && m_pStream->m_pTransport != nullptr) {
            m_IsRegistered = !m_pStream->m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_pStream->m_Type), PORT);
        }
        m_pStream->m_pTransport = pTransport;
        if (m_IsEnabled) {
            m_IsRegistered = pTransport->m_StreamManager.RegisterStream(m_pStream, PORT, nullptr);
        }
    }

    bool m_IsEnabled;        // 0x04
    bool m_IsRegistered;     // 0x05
    PortMap* m_pPortMap;     // 0x08
    RoutingStream* m_pStream; // 0x0C
};
ASSERT_SIZE(Router, 0x10);
} // namespace nex
} // namespace nn

namespace __rw {
// (the range erases of the port maps are out of line; nex_Router.cpp)
template <>
DECOMP_NOINLINE __rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>,
                          nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::iterator
__rb_tree<u16, std::pair<const u16, nn::nex::InetAddress>, __select1st<std::pair<const u16, nn::nex::InetAddress>, u16>, std::less<u16>,
          nn::nex::MemAllocator<std::pair<const u16, nn::nex::InetAddress> > >::erase(iterator first, iterator last);
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>,
                          std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::iterator
__rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, u16>, __select1st<std::pair<const nn::nex::InetAddress, u16>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>,
          nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, u16> > >::erase(iterator first, iterator last);
} // namespace __rw
