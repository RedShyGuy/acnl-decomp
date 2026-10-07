#include "nn/pia/inet/inet_SocketInputStream.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_PacketOld.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/common/common_String.h"
#include "nn/pia/inet/inet_Socket.h"

namespace nn {
namespace pia {
namespace inet {
// 0x0097E420
nn::pia::common::InetAddress nn::pia::inet::SocketInputStream::s_IgnoreAddress;

// 0x003E8568 (name is ours)
nn::Result nn::pia::inet::SocketInputStream::readPacket(nn::pia::common::Packet* pPacket)
{
    common::InetAddress address;
    u8 ttl = 0;
    nn::Result result = m_pSocket->RecvFrom(reinterpret_cast<u8*>(pPacket), BUFFER_SIZE, &address, &ttl, reinterpret_cast<int*>(&pPacket->m_Size));
    if (result.IsFailure()) {
        if (!m_IsStarted) {
            return common::RESULT_NO_DATA;
        }
        return result;
    }
    if (!address.IsValid()) {
        // (the address was printed by a removed log call)
        common::String string;
        address.GetAddressString(&string);
        return common::RESULT_NO_DATA;
    }
    if (address.GetKey() == s_IgnoreAddress.GetKey()) {
        return common::RESULT_NO_DATA;
    }
    common::StationAddress stationAddress;
    stationAddress.SetInetAddress(address);
    pPacket->m_SourceStationAddress = stationAddress;
    pPacket->m_Ttl = ttl;
    // (a trace call was removed by the linker here)
    return nn::Result();
}

// 0x003E86D8 (name is ours)
nn::Result nn::pia::inet::SocketInputStream::readPacketOld(nn::pia::common::PacketOld* pPacket)
{
    common::InetAddress address;
    int size = 0;
    u8 ttl = 0;
    nn::Result result = m_pSocket->RecvFrom(m_Buffer, BUFFER_SIZE, &address, &ttl, &size);
    if (result.IsFailure()) {
        if (!m_IsStarted) {
            return common::RESULT_NO_DATA;
        }
        return result;
    }
    if (!address.IsValid()) {
        // (the address was printed by a removed log call)
        common::String string;
        address.GetAddressString(&string);
        return common::RESULT_NO_DATA;
    }
    if (address.GetKey() == s_IgnoreAddress.GetKey()) {
        return common::RESULT_NO_DATA;
    }
    result = pPacket->Deserialize(m_Buffer, size);
    if (result.IsFailure()) {
        if (!common::PacketOld::IsPacketOld(m_Buffer)) {
            return common::RESULT_NO_DATA;
        }
        // (a trace call was removed by the linker here)
        return result;
    }
    common::StationAddress stationAddress;
    stationAddress.SetInetAddress(address);
    pPacket->SetSourceStationAddress(stationAddress);
    pPacket->m_Ttl = ttl;
    // (a trace call was removed by the linker here)
    return nn::Result();
}

// 0x003E8898 | fefates:callgraph [tier C]
void nn::pia::inet::SocketInputStream::AddIgnoreAddress(const nn::pia::common::InetAddress& address)
{
    s_IgnoreAddress = address;
}

// 0x003E88A8 slot 0x0C
// 0x00774690 (thunk)
nn::Result nn::pia::inet::SocketInputStream::Read(nn::pia::common::Packet* pPacket)
{
    if (!m_IsStarted) {
        return common::RESULT_NO_DATA;
    }
    return readPacket(pPacket);
}

// 0x003E88C4 slot 0x08 (name is ours)
nn::Result nn::pia::inet::SocketInputStream::ReadOld(nn::pia::common::PacketOld* pPacket)
{
    if (!m_IsStarted) {
        return common::RESULT_NO_DATA;
    }
    return readPacketOld(pPacket);
}

// 0x003E88E0 | fefates:bytes [tier B]
nn::pia::inet::SocketInputStream::SocketInputStream()
{
    // only the base and the vptrs (in the original too)
}

// 0x003E7FFC
// 0x003E8900 (deleting dtor)
// 0x007746C4 (thunk)
// 0x007746B0 (deleting thunk)
nn::pia::inet::SocketInputStream::~SocketInputStream()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
