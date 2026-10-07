#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/inet/inet_SocketStreamBase.h"

namespace nn {
namespace pia {
namespace common {
class Packet;
class PacketOld;
} // namespace common
namespace inet {
// RTTI N2nn3pia4inet17SocketInputStreamE @ 0x008CF8CC
// vtable 0x00900124 (vptr 0x0090012C), offset_to_top 0, 4 entries
// vtable 0x0090013C (vptr 0x00900144), offset_to_top -1376, 3 entries
//
// Reads the packets from the socket without blocking: the current ones straight into the packet,
// the old ones through the buffer. Packets from the ignored address are dropped. The names marked
// so are ours.
class SocketInputStream : public ::nn::pia::inet::SocketStreamBase, public ::nn::pia::common::IPacketInput
{
public:
    SocketInputStream(); // 0x003E88E0 | fefates:bytes [tier B]
    virtual ~SocketInputStream(); // 0x003E7FFC slot 0x00
    // 0x003E8900 slot 0x04 (deleting dtor)
    // RESULT_NO_DATA when stopped or nothing arrived (name is ours)
    virtual nn::Result ReadOld(nn::pia::common::PacketOld* pPacket); // 0x003E88C4 slot 0x08
    virtual nn::Result Read(nn::pia::common::Packet* pPacket); // 0x003E88A8 slot 0x0C

    // the packets from the address are dropped (e.g. the own ones)
    static void AddIgnoreAddress(const nn::pia::common::InetAddress& address); // 0x003E8898 | fefates:callgraph [tier C]

    // (names are ours)
    DECOMP_NOINLINE nn::Result readPacket(nn::pia::common::Packet* pPacket); // 0x003E8568
    DECOMP_NOINLINE nn::Result readPacketOld(nn::pia::common::PacketOld* pPacket); // 0x003E86D8

    static common::InetAddress s_IgnoreAddress;
};
ASSERT_SIZE(SocketInputStream, 0x564);
} // namespace inet
} // namespace pia
} // namespace nn
