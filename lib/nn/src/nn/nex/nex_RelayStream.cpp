#include "nn/nex/nex_RelayStream.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_PacketBufferManager.h"
#include "nn/nex/nex_PacketBufferPacketOut.h"
#include "nn/nex/nex_PacketOut.h"
#include "nn/nex/nex_RootTransport.h"

#include <new>

namespace nn {
namespace nex {
namespace {
// the message version of the relay packets
const u8 MESSAGE_VERSION_RELAY = 0xD0;
// the packet type of the relay packets
const u32 PACKET_TYPE_RELAY = 15;
} // namespace

// 0x0035B694 (name is ours)
nn::nex::PacketOut* nn::nex::RelayStream::CreatePacket(nn::nex::Buffer* pPayload, unsigned char relayType)
{
    PacketOut* pPacket;
    if (pPayload != nullptr && (pPayload->m_Flags & Buffer::FLAG_USE_POOL)) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(PacketBufferPacketOut), false);
        if (pMemory == nullptr) {
            return nullptr;
        }
        pPacket = ::new (pMemory) PacketBufferPacketOut(nullptr, static_cast<PacketType>(PACKET_TYPE_RELAY), 0, pPayload, 0);
        if (pPacket == nullptr) {
            return nullptr;
        }
        if ((pPacket->m_TypeFlags & PacketOut::FLAG_NEED_ACK) && pPacket->m_pTimeout == nullptr) {
            delete pPacket;
            return nullptr;
        }
    } else {
        pPacket = new PacketOut(nullptr, static_cast<PacketType>(PACKET_TYPE_RELAY), 0, pPayload, 0);
    }
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    header.m_Unknown0x00 = 0;
    header.m_TypeFlags = 0;
    header.m_Unknown0x04 = 0;
    header.m_Unknown0x08 = 0;
    header.m_Unknown0x0C = 0;
    header.m_Unknown0x0E = 0;
    header.m_Unknown0x10 = 0;
    header.m_Unknown0x14 = 0;
    header.m_TimeStamp = 0;
    pPacket->m_MessageVersion = MESSAGE_VERSION_RELAY;
    header.m_TypeFlags = relayType & 0xF;
    return pPacket;
}

// 0x0035B774 slot 0x08 | fefates:bytes
bool nn::nex::RelayStream::ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket)
{
    Packet::RelayHeader& header = pPacket->GetRelayHeader();
    Buffer* pData = pPacket->m_pPayload;
    InetAddress source(header.m_Unknown0x04, header.m_Unknown0x0C);
    switch (header.m_TypeFlags & 0xF) {
    case RELAY_TYPE_DATA:
        m_RoutingTable.Add(source, pPacket->m_Address, true);
        if (!m_pTransport->Receive(port, pData, &source)) {
            m_RoutingTable.Remove(source);
            return false;
        }
        break;
    case RELAY_TYPE_PING:
        if (header.m_Unknown0x1C != 0) {
            return false;
        }
        if (m_pPingCallback != nullptr) {
            m_pPingCallback(&m_PingContext, &pPacket->m_Address, port, header.m_TimeStamp, &header.m_Unknown0x1C, pData);
        }
        break;
    default:
        return false;
    }
    pPacket->ReleaseRef();
    return true;
}

// 0x0036037C slot 0x0C
void nn::nex::RelayStream::DoWork()
{
    m_RoutingTable.Update();
}

// 0x0035B9A4 slot 0x00
// 0x0035B8B4 (deleting dtor)
nn::nex::RelayStream::~RelayStream()
{
    // nothing to do: the members are destroyed by the compiler
}

} // namespace nex
} // namespace nn
