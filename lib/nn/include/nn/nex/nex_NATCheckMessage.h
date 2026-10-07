#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The message of the NAT check (16 bytes, big endian on the network) that pia::inet::NatDetecter
// sends to the NAT check servers and receives back from them. The type name and
// ToNetworkByteOrder are from the symbols; the layout is from NatDetecter, the member names and
// ToHostByteOrder are ours.
class NATCheckMessage
{
public:
    // the types of the replies of the servers (NatDetecter keeps one of each)
    static const u32 TYPE_REPLY_MIN = 101;
    static const u32 TYPE_REPLY_NUM = 3;

    NATCheckMessage(); // 0x0037A694 (name is ours)

    void ToNetworkByteOrder(); // 0x0037A62C | fefates:bytes [tier C]
    void ToHostByteOrder(); // 0x0037A660 (name is ours)

    u32 m_Type;        // 0x0
    // the address and port the server saw the message come from
    u32 m_Port;        // 0x4
    u32 m_Address;     // 0x8
    u32 m_Unknown0xC;  // 0xC
};
ASSERT_SIZE(NATCheckMessage, 0x10);
} // namespace nex
} // namespace nn
