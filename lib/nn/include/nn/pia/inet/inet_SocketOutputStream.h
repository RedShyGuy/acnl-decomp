#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/inet/inet_SocketAddress.h"
#include "nn/pia/inet/inet_SocketStreamBase.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace common {
class InetAddress;
class Packet;
class PacketOld;
} // namespace common
namespace inet {
// RTTI N2nn3pia4inet18SocketOutputStreamE @ 0x008CF8F8
// vtable 0x009001BC (vptr 0x009001C4), offset_to_top 0, 8 entries
// vtable 0x009001E4 (vptr 0x009001EC), offset_to_top -1376, 6 entries
//
// Writes the packets to the socket: to one address or, with the addresses of the connected
// stations it keeps (OnStationConnectionEvent), to several at once. The names marked so are ours.
class SocketOutputStream : public ::nn::pia::inet::SocketStreamBase, public ::nn::pia::common::IPacketOutput
{
public:
    SocketOutputStream(); // 0x003F8A3C | fefates:bytes [tier B]
    virtual ~SocketOutputStream(); // 0x003F8A6C slot 0x00
    // 0x003F8A5C slot 0x04 (deleting dtor)
    virtual nn::Result OnStationConnectionEvent(); // 0x003F85F4 slot 0x08 | fefates:bytes
    // (names are ours)
    virtual nn::Result WriteOld(const nn::pia::common::PacketOld& packet); // 0x003F8868 slot 0x0C
    virtual nn::Result Write(const nn::pia::common::Packet& packet); // 0x003F86BC slot 0x10 | fefates:callseq
    virtual u32 GetDestinationNumMax(); // 0x0072F160 slot 0x14
    virtual bool IsBroadcast(); // 0x0072F158 slot 0x18
    // to all connected stations (name is ours)
    virtual nn::Result BroadcastOld(const nn::pia::common::PacketOld& packet); // 0x003F8420 slot 0x1C

    nn::Result SendToMulti(const void* pData, unsigned int size, const nn::pia::inet::SockAddrIn* pAddresses, int addressNum); // 0x003F8578 | fefates:bytes [tier B]
    // with the TTL if it is not 0
    nn::Result SendTo(const void* pData, unsigned int size, const nn::pia::common::InetAddress& address, unsigned char ttl); // 0x003F894C | fefates:bytes [tier B]

    // (inline; name is ours) the addresses of the stations in the bitmap, without the local one
    int GetDestinations(SockAddrIn* pAddresses, u32 bitmap, StationIndex localStationIndex) const
    {
        int num = 0;
        for (u32 i = 0; i < STATION_INDEX_MAX + 1; i++) {
            if (i == localStationIndex) {
                continue;
            }
            if ((bitmap >> i) & 1 && m_StationAddresses[i].m_Address != 0 && m_StationAddresses[i].m_Port != 0) {
                pAddresses[num++] = m_StationAddresses[i];
            }
        }
        return num;
    }

    SockAddrIn m_StationAddresses[STATION_INDEX_MAX + 1]; // 0x564, of the connected stations
};
ASSERT_OFFSET(SocketOutputStream, m_StationAddresses, 0x564);
ASSERT_SIZE(SocketOutputStream, 0x5C4);
} // namespace inet
} // namespace pia
} // namespace nn
