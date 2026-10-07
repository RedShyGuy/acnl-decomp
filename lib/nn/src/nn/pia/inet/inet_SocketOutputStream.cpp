#include "nn/pia/inet/inet_SocketOutputStream.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_PacketOld.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_Socket.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"
#include <string.h>

namespace nn {
namespace pia {
namespace inet {
namespace {
// the destinations a packet may have (IPacketOutput::GetDestinationNumMax)
const u32 DESTINATION_NUM_MAX = 16;
// PacketOld::m_Unknown0xA of a packet to all stations
const u8 BROADCAST_STATION_INDEX = 0xFF;
} // namespace

// 0x003F8420 slot 0x1C (name is ours)
nn::Result nn::pia::inet::SocketOutputStream::BroadcastOld(const nn::pia::common::PacketOld& packet)
{
    if (!m_IsStarted) {
        return nn::Result();
    }
    u32 size;
    nn::Result result = packet.Serialize(m_Buffer, &size, BUFFER_SIZE);
    if (result.IsFailure()) {
        return result;
    }
    SockAddrIn addresses[STATION_INDEX_MAX + 1];
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return nn::Result();
    }
    int num = GetDestinations(addresses, 0xFFFFFFFF, pLocalStation->m_StationIndex);
    if (num == 0) {
        return nn::Result();
    }
    result = SendToMulti(m_Buffer, size, addresses, num);
    if (result.IsFailure()) {
        // (a trace call of the packet was removed by the linker here)
        return result;
    }
    return nn::Result();
}

// 0x003F8578 | fefates:bytes [tier B]
nn::Result nn::pia::inet::SocketOutputStream::SendToMulti(const void* pData, unsigned int size, const nn::pia::inet::SockAddrIn* pAddresses, int addressNum)
{
    int sentSize = 0;
    nn::Result result = m_pSocket->SendToMulti(pData, size, pAddresses, addressNum, &sentSize);
    if (result.IsFailure()) {
        if (m_IsStarted) {
            return result;
        }
        return nn::Result();
    }
    if (sentSize != 0 && sentSize != static_cast<int>(size)) {
        return common::RESULT_INVALID_STATE;
    }
    return nn::Result();
}

// 0x003F85F4 slot 0x08 | fefates:bytes
// 0x003F85EC (thunk)
nn::Result nn::pia::inet::SocketOutputStream::OnStationConnectionEvent()
{
    if (!m_IsStarted) {
        return nn::Result();
    }
    memset(m_StationAddresses, 0, sizeof(m_StationAddresses));
    for (transport::Station** it = transport::StationManager::s_pInstance->m_ActiveStations.Begin();
         it != transport::StationManager::s_pInstance->m_ActiveStations.End(); it++) {
        transport::Station* pStation = *it;
        if (pStation->m_State != transport::Station::STATION_STATE_CONNECTED || pStation->m_StationIndex > STATION_INDEX_MAX) {
            continue;
        }
        StationIndex stationIndex = pStation->m_StationIndex;
        SocketAddress socketAddress;
        socketAddress.SetInetAddress(pStation->m_StationAddress.GetInetAddress());
        m_StationAddresses[stationIndex] = socketAddress.GetSockAddrIn();
    }
    return nn::Result();
}

// 0x003F86BC slot 0x10 | fefates:callseq
// 0x003F86B4 (thunk)
nn::Result nn::pia::inet::SocketOutputStream::Write(const nn::pia::common::Packet& packet)
{
    if (!m_IsStarted) {
        return nn::Result();
    }
    common::InetAddress address(packet.m_DestinationStationAddress.GetInetAddress());
    nn::Result result;
    if (address.IsValid()) {
        result = SendTo(&packet, packet.m_Size, address, packet.m_Ttl);
        if (result.IsFailure()) {
            // (a trace call of the packet was removed by the linker here)
            return result;
        }
        return nn::Result();
    }
    // to the stations of the bitmap
    u32 bitmap = packet.m_DestinationBitmap;
    SockAddrIn addresses[STATION_INDEX_MAX + 1];
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    if (pLocalStation == nullptr) {
        return nn::Result();
    }
    int num = GetDestinations(addresses, bitmap, pLocalStation->m_StationIndex);
    if (num == 0) {
        return nn::Result();
    }
    result = SendToMulti(&packet, packet.m_Size, addresses, num);
    if (result.IsFailure()) {
        // (a trace call of the packet was removed by the linker here)
        return result;
    }
    return nn::Result();
}

// 0x003F8868 slot 0x0C (name is ours)
nn::Result nn::pia::inet::SocketOutputStream::WriteOld(const nn::pia::common::PacketOld& packet)
{
    if (!m_IsStarted) {
        return nn::Result();
    }
    if (packet.m_Unknown0xA == BROADCAST_STATION_INDEX) {
        return BroadcastOld(packet);
    }
    u32 size;
    nn::Result result = packet.Serialize(m_Buffer, &size, BUFFER_SIZE);
    if (result.IsFailure()) {
        return result;
    }
    result = SendTo(m_Buffer, size, packet.m_DestinationAddress, packet.m_Ttl);
    if (result.IsSuccess()) {
        return nn::Result();
    }
    // (a trace call of the packet was removed by the linker here)
    return result;
}

// 0x003F894C | fefates:bytes [tier B]
nn::Result nn::pia::inet::SocketOutputStream::SendTo(const void* pData, unsigned int size, const nn::pia::common::InetAddress& address, unsigned char ttl)
{
    int sentSize = 0;
    if (ttl != 0) {
        nn::Result result = m_pSocket->SetTtl(ttl);
        if (result.IsFailure()) {
            return m_IsStarted ? result : nn::Result();
        }
    }
    nn::Result result = m_pSocket->SendTo(pData, size, address, &sentSize);
    if (ttl != 0) {
        nn::Result ttlResult = m_pSocket->SetTtl(0);
        if (ttlResult.IsFailure()) {
            return m_IsStarted ? ttlResult : nn::Result();
        }
    }
    if (result.IsFailure()) {
        return m_IsStarted ? result : nn::Result();
    }
    if (sentSize != 0 && sentSize != static_cast<int>(size)) {
        return common::RESULT_INVALID_STATE;
    }
    return nn::Result();
}

// 0x003F8A3C | fefates:bytes [tier B]
nn::pia::inet::SocketOutputStream::SocketOutputStream()
{
    // only the base and the vptrs (in the original too)
}

// 0x003F8A6C
// 0x003F8A5C (deleting dtor)
// 0x007746E0 (thunk)
// 0x007746CC (deleting thunk)
nn::pia::inet::SocketOutputStream::~SocketOutputStream()
{
    // empty (in the original too)
}

// 0x0072F158 slot 0x18
// 0x007746E8 (thunk)
bool nn::pia::inet::SocketOutputStream::IsBroadcast()
{
    return false;
}

// 0x0072F160 slot 0x14
// 0x007746F0 (thunk)
u32 nn::pia::inet::SocketOutputStream::GetDestinationNumMax()
{
    return DESTINATION_NUM_MAX;
}

} // namespace inet
} // namespace pia
} // namespace nn
