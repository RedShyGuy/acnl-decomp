#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class InetAddress;
} // namespace common
namespace inet {
// an IPv4 address as the socket service takes it (big endian; the type name is from the symbols,
// the members are ours)
struct SockAddrIn
{
    u8 m_Length;   // 0x0 (8)
    u8 m_Family;   // 0x1 (AF_INET)
    u16 m_Port;    // 0x2
    u32 m_Address; // 0x4
};
ASSERT_SIZE(SockAddrIn, 0x8);

// the hints and results of SocketAddress::GetAddressInfo, laid out like nn::socket::AddrInfo (the
// type name is from the symbols, the members are ours)
struct AddrInfo
{
    s32 m_Flags;               // 0x00
    s32 m_Family;              // 0x04
    s32 m_SockType;            // 0x08
    s32 m_Protocol;            // 0x0C
    s32 m_AddrLength;          // 0x10
    char* m_pCanonName;        // 0x14
    SockAddrIn* m_pAddr;       // 0x18
    AddrInfo* m_pNext;         // 0x1C
};
ASSERT_SIZE(AddrInfo, 0x20);

// A socket address (a SockAddrIn and a pointer to it, as the socket functions take it). The
// layout is from Init; the member names are ours.
class SocketAddress
{
public:
    // (inline; the static initializer of the NAT server addresses calls Init)
    SocketAddress() { Init(); }

    void Init(); // 0x003E6C7C | fefates:bytes [tier B]
    // the address of the host name; false if it cannot be resolved
    bool GetAddressInfo(const char* pNode, const char* pService, const nn::pia::inet::AddrInfo* pHints); // 0x003E6BC4 | fefates:bytes [tier B]
    void GetInetAddress(nn::pia::common::InetAddress* pAddress); // 0x003E6C10 | fefates:bytes [tier B]
    void SetInetAddress(const nn::pia::common::InetAddress& address); // 0x003E6C50 | fefates:bytes [tier B]
    SockAddrIn GetSockAddrIn(); // 0x003E6BA4 | fefates:callgraph [tier C]
    void SetSockAddrIn(const nn::pia::inet::SockAddrIn& sockAddrIn); // 0x003E6BB4 | fefates:callgraph [tier C]

    SockAddrIn* m_pSockAddrIn; // 0x0, m_SockAddrIn
    SockAddrIn m_SockAddrIn;   // 0x4
};
ASSERT_SIZE(SocketAddress, 0xC);
} // namespace inet
} // namespace pia
} // namespace nn
