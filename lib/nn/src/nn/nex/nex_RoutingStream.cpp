#include "nn/nex/nex_RoutingStream.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_ByteStream.h"
#include "nn/nex/nex_PacketBuffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_PacketBufferPacketOut.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Router.h"

#include <new>

namespace nn {
namespace nex {
// 0x0096C9E8 (name is ours)
u32 RoutingStream::s_CleanupPeriod;
// 0x0096C9EC (name is ours)
u32 RoutingStream::s_RouteTimeout = 10000;
// 0x0096C9F0 (name is ours)
u32 RoutingStream::s_MaxRoutes;

namespace {
// the type of the stream
const u32 TYPE = 8;
// the header of a routing packet: the address and the port
const u32 HEADER_SIZE = 6;

// (inline) the address of the forwarded packets whose port is in the port map of the router
inline const InetAddress& GetMappedAddress()
{
    static InetAddress s_MappedAddress(L"255.0.0.0", 0);
    return s_MappedAddress;
}
} // namespace

// 0x0036BFB0 (name is ours)
u32 nn::nex::RoutingStream::GetRouteCount(const nn::nex::InetAddress* pAddress)
{
    if (pAddress == nullptr) {
        return m_Routes.size();
    }
    ScopedCS lock(m_Lock);
    u32 count = 0;
    for (qMap<Route, Time>::iterator it = m_Routes.begin(); it != m_Routes.end(); ++it) {
        if (it->first.first.IsSame(*pAddress) || it->first.second.IsSame(*pAddress)) {
            count++;
        }
    }
    return count;
}

// 0x0036C0AC (name is ours)
nn::nex::PacketOut* nn::nex::RoutingStream::CreatePacket(nn::nex::Buffer* pPayload, unsigned int type, const nn::nex::InetAddress* pDestination)
{
    Buffer* pBuffer;
    if (pPayload != nullptr && (pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBuffer), false);
        if (pMemory == nullptr) {
            return nullptr;
        }
        pBuffer = ::new (pMemory) PacketBuffer(pPayload->m_ContentSize + HEADER_SIZE);
        if (pBuffer == nullptr) {
            return nullptr;
        }
        if (pBuffer->m_pData == nullptr) {
            delete pBuffer;
            return nullptr;
        }
    } else {
        pBuffer = new Buffer();
    }
    ByteStream stream(pBuffer);
    pBuffer->ReleaseRef();
    u32 address = __builtin_bswap32(pDestination->m_SockAddr.m_Address);
    stream.m_pBuffer->AppendData(&address, sizeof(address), static_cast<u32>(-1));
    u16 port = __builtin_bswap16(pDestination->m_SockAddr.m_Port);
    stream.m_pBuffer->AppendData(&port, sizeof(port), static_cast<u32>(-1));
    stream.m_pBuffer->AppendData(pPayload->m_pData + pPayload->m_HeaderSize, pPayload->m_ContentSize, static_cast<u32>(-1));
    PacketOut* pPacket;
    if (pPayload != nullptr && (pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
        if (pMemory == nullptr) {
            return nullptr;
        }
        pPacket = ::new (pMemory) PacketBufferPacketOut(nullptr, static_cast<PacketType>(type), 0, stream.m_pBuffer, 0);
        if (pPacket == nullptr) {
            return nullptr;
        }
        if ((pPacket->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
            delete pPacket;
            return nullptr;
        }
    } else {
        pPacket = new PacketOut(nullptr, static_cast<PacketType>(type), 0, stream.m_pBuffer, 0);
    }
    pPacket->m_MessageVersion = 0;
    return pPacket;
}

// 0x0036C2DC slot 0x08 | fefates:callseq
bool nn::nex::RoutingStream::ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket)
{
    Router* pRouter = m_pTransport->GetRouter();
    Buffer* pPayload = pPacket->m_pPayload;
    if (pPayload == nullptr || pPayload->m_ContentSize <= 12) {
        return false;
    }
    {
        // the address of the routing header and the data behind it
        InetAddress address;
        Buffer data;
        {
            ByteStream stream(pPayload);
            u32 hostAddress;
            u16 hostPort;
            stream.ExtractPrimitive(&hostAddress);
            stream.ExtractPrimitive(&hostPort);
            address.m_SockAddr.m_Address = __builtin_bswap32(hostAddress);
            address.m_SockAddr.m_Port = __builtin_bswap16(hostPort);
            data.AppendData(stream.m_pBuffer->m_pData + stream.m_pBuffer->m_HeaderSize + stream.m_Position, stream.m_pBuffer->m_ContentSize - stream.m_Position,
                            static_cast<u32>(-1));
        }
        switch (pPacket->m_TypeFlags & 0xF) {
        case PACKET_TYPE_DATA: {
            // data for this station from the address
            InetAddress source = address;
            m_RoutingTable.Add(source, pPacket->m_Address, true);
            if (!m_pTransport->Receive(port, &data, &source)) {
                m_RoutingTable.Remove(source);
                return false;
            }
            break;
        }
        case PACKET_TYPE_FORWARD: {
            // data from the sender for the address
            InetAddress destination;
            InetAddress source;
            if (__builtin_bswap32(address.m_SockAddr.m_Address) == __builtin_bswap32(GetMappedAddress().m_SockAddr.m_Address)) {
                qMap<u16, InetAddress>::iterator it = pRouter->m_pPortMap->m_Addresses.find(__builtin_bswap16(address.m_SockAddr.m_Port));
                if (it != pRouter->m_pPortMap->m_Addresses.end()) {
                    destination.m_SockAddr = it->second.m_SockAddr;
                }
                u16 mappedPort;
                qMap<InetAddress, u16>::iterator it2 = pRouter->m_pPortMap->m_Ports.find(pPacket->m_Address);
                if (it2 != pRouter->m_pPortMap->m_Ports.end()) {
                    mappedPort = it2->second;
                }
                source.m_SockAddr.m_Address = __builtin_bswap32(__builtin_bswap32(GetMappedAddress().m_SockAddr.m_Address));
                source.m_SockAddr.m_Port = __builtin_bswap16(mappedPort);
                wchar_t string[32];
                destination.ToStr(string);
                source.ToStr(string);
            } else {
                destination.m_SockAddr = address.m_SockAddr;
                source.m_SockAddr = pPacket->m_Address.m_SockAddr;
            }
            bool isAccepted = true;
            if (s_CleanupPeriod != 0 && m_Peers.find(address) == m_Peers.end() && m_Peers.find(source) == m_Peers.end()) {
                // the forwarded routes are counted
                ScopedCS lock(m_Lock);
                Time now = SystemClock::GetTime();
                Route route(address, source);
                qMap<Route, Time>::iterator it = m_Routes.find(route);
                if (it != m_Routes.end()) {
                    it->second = now;
                } else if (s_MaxRoutes != 0 && GetRouteCount(nullptr) >= s_MaxRoutes) {
                    isAccepted = false;
                } else {
                    m_Routes.IndexInline(route) = now;
                }
            }
            if (m_pFilter != nullptr) {
                isAccepted = m_pFilter(&source, &destination, pPacket);
            }
            if (!isAccepted) {
                return false;
            }
            PacketOut* pOut = CreatePacket(&data, PACKET_TYPE_DATA, &source);
            if (pOut != nullptr) {
                if ((pOut->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pOut->m_pTimeout == nullptr) {
                    delete pOut;
                } else {
                    pOut->m_Address.m_SockAddr = destination.m_SockAddr;
                    m_pTransport->SendPacket(port, m_Type, Router::PORT, Router::PORT, pOut, false);
                    pOut->ReleaseRef();
                }
            }
            break;
        }
        default:
            return false;
        }
    }
    pPacket->ReleaseRef();
    return true;
}

// 0x0036CDC0 (name is ours)
void nn::nex::RoutingStream::RemoveExpiredRoutes()
{
    ScopedCS lock(m_Lock);
    Time now = SystemClock::GetTime();
    for (qMap<Route, Time>::iterator it = m_Routes.begin(); it != m_Routes.end();) {
        if (static_cast<s64>(now.m_Value - it->second.m_Value) > static_cast<s64>(s_RouteTimeout)) {
            m_Routes.erase(it++);
        } else {
            ++it;
        }
    }
    for (qMap<InetAddress, Time>::iterator it = m_Peers.begin(); it != m_Peers.end();) {
        if (static_cast<s64>(now.m_Value - it->second.m_Value) > static_cast<s64>(s_RouteTimeout)) {
            m_Peers.erase(it++);
        } else {
            ++it;
        }
    }
}

// 0x0036D004 slot 0x0C
void nn::nex::RoutingStream::DoWork()
{
    if (s_CleanupPeriod == 0) {
        return;
    }
    Time now = SystemClock::GetTime();
    if (static_cast<s64>(now.m_Value - m_LastCleanup.m_Value) > static_cast<s64>(s_CleanupPeriod)) {
        m_LastCleanup = now;
        RemoveExpiredRoutes();
    }
}

// 0x0036D064 | fefates:bytes-fuzzy [tier B]
nn::nex::RoutingStream::RoutingStream()
    : Stream(static_cast<Type>(TYPE)), m_Lock(1), m_RoutingTable(GetSettings(static_cast<Type>(1))->m_MaxSilenceTime * 3), m_pTransport(nullptr), m_pFilter(nullptr)
{
    m_LastCleanup = Time(0);
}

// 0x0036D22C slot 0x00 | fefates:callseq
// 0x0036D1FC (deleting dtor)
nn::nex::RoutingStream::~RoutingStream()
{
    // nothing to do: the members are destroyed by the compiler
}

} // namespace nex
} // namespace nn

namespace __rw {
// the functions of the trees of the routing stream
// 0x00803108 (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>,
                          __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>,
                          nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::iterator
__rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>,
          __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>,
          nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x007F8AAC (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::Time>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::Time>, nn::nex::InetAddress>,
                          std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::Time> > >::iterator
__rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::Time>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::Time>, nn::nex::InetAddress>,
          std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::Time> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
// 0x00802AAC (name is ours)
template __rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::iterator __rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::erase(iterator it);
// 0x00803284 (name is ours)
template __rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::iterator __rb_tree<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, __select1st<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>, std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, std::less<std::pair<nn::nex::InetAddress, nn::nex::InetAddress>>, nn::nex::MemAllocator<std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const std::pair<nn::nex::InetAddress, nn::nex::InetAddress>, nn::nex::Time>& value);
// 0x007F8450 (name is ours)
template __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::Time>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::Time>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::Time> > >::iterator __rb_tree<nn::nex::InetAddress, std::pair<const nn::nex::InetAddress, nn::nex::Time>, __select1st<std::pair<const nn::nex::InetAddress, nn::nex::Time>, nn::nex::InetAddress>, std::less<nn::nex::InetAddress>, nn::nex::MemAllocator<std::pair<const nn::nex::InetAddress, nn::nex::Time> > >::erase(iterator it);
} // namespace __rw
