#include "nn/nex/nex_Relay.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_RelayMessage.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}
} // namespace

// 0x003CEDB4 | fefates:bytes [tier B]
bool nn::nex::Relay::ShouldRelay(const nn::nex::InetAddress* pAddress)
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

// 0x003CEE54 (name is ours)
bool nn::nex::Relay::Send(nn::nex::Buffer* pData, const nn::nex::InetAddress* pAddress, unsigned short port)
{
    InetAddress via;
    m_pStream->m_RoutingTable.Find(*pAddress, via);
    PacketOut* pPacket = RelayStream::CreatePacket(pData, RelayStream::RELAY_TYPE_DATA);
    if (pPacket == nullptr) {
        return false;
    }
    if ((pPacket->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
        delete pPacket;
        return false;
    }
    pPacket->m_Address.m_SockAddr = via.m_SockAddr;
    pPacket->m_Unknown0x130 = true;
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    header.m_Unknown0x04 = __builtin_bswap32(m_Address.m_SockAddr.m_Address);
    header.m_Unknown0x0C = __builtin_bswap16(m_Address.m_SockAddr.m_Port);
    header.m_Unknown0x10 = m_pStream->m_Unknown0x40;
    header.m_Unknown0x14 = m_pStream->m_Unknown0x44;
    header.m_Unknown0x08 = __builtin_bswap32(pAddress->m_SockAddr.m_Address);
    header.m_Unknown0x0E = __builtin_bswap16(pAddress->m_SockAddr.m_Port);
    bool result = m_pStream->m_pTransport->SendPacket(port, m_pStream->m_Type, PORT, PORT, pPacket, true);
    pPacket->ReleaseRef();
    return result;
}

// 0x003CEFC4 | fefates:callgraph [tier C]
void nn::nex::Relay::EnableRelay(bool enable)
{
    m_IsEnabled = enable;
    if (enable) {
        if (m_pStream->m_pTransport != nullptr) {
            m_IsRegistered = m_pStream->m_pTransport->m_StreamManager.RegisterStream(m_pStream, PORT, nullptr);
        }
        m_pNetwork = Network::GetInstance();
        return;
    }
    if (!m_IsRegistered) {
        return;
    }
    if (m_pStream->m_pTransport != nullptr) {
        m_IsRegistered = !m_pStream->m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_pStream->m_Type), PORT);
    }
    Clear();
}

// 0x003CF054 (name is ours)
void nn::nex::Relay::Clear()
{
    ScopedCS lock(GetSchedulerLock());
    m_pStream->m_RoutingTable.m_Entries.clear();
    m_pStream->m_Unknown0x44 = -1;
    m_pStream->m_Unknown0x40 = -1;
    if (Network::GetInstance() == nullptr) {
        return;
    }
    if (Network::GetInstance()->m_pNATTraversalEngine != nullptr) {
        NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
        pEngine->m_RelayHost = String("");
        Network::GetInstance()->m_pNATTraversalEngine->m_RelayPort = 0;
    }
    if (&Network::GetInstance()->m_MessageSelector != nullptr) {
        Network::GetInstance()->m_MessageSelector.m_pRelayMessage->m_Key.m_Size = 0;
    }
}

// 0x003CF1E4 slot 0x00
// 0x003CF154 (deleting dtor)
nn::nex::Relay::~Relay()
{
    if (m_IsRegistered) {
        if (m_pStream->m_pTransport != nullptr) {
            m_IsRegistered = !m_pStream->m_pTransport->m_StreamManager.UnregisterStream(static_cast<Stream::Type>(m_pStream->m_Type), PORT);
        }
        Clear();
    }
    delete m_pStream;
}

} // namespace nex
} // namespace nn
