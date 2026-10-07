#include "nn/pia/inet/inet_SocketAddress.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/socket/socket_Api.h"
#include <string.h>

namespace nn {
namespace pia {
namespace inet {
namespace {
// (inline; names are ours)
inline u32 SwapU32(u32 value)
{
    return __builtin_bswap32(value);
}

inline u16 SwapU16(u16 value)
{
    return static_cast<u16>((value << 8) | (value >> 8));
}
} // namespace

// 0x003E6BA4 | fefates:callgraph [tier C]
nn::pia::inet::SockAddrIn nn::pia::inet::SocketAddress::GetSockAddrIn()
{
    return *m_pSockAddrIn;
}

// 0x003E6BB4 | fefates:callgraph [tier C]
void nn::pia::inet::SocketAddress::SetSockAddrIn(const nn::pia::inet::SockAddrIn& sockAddrIn)
{
    *m_pSockAddrIn = sockAddrIn;
}

// 0x003E6BC4 | fefates:bytes [tier B]
bool nn::pia::inet::SocketAddress::GetAddressInfo(const char* pNode, const char* pService, const nn::pia::inet::AddrInfo* pHints)
{
    bool isFound = false;
    nn::socket::AddrInfo* pResult;
    if (nn::socket::GetAddrInfo(pNode, pService, reinterpret_cast<const nn::socket::AddrInfo*>(pHints), &pResult) == 0) {
        isFound = true;
        m_pSockAddrIn->m_Address = reinterpret_cast<const SockAddrIn*>(pResult->addr)->m_Address;
        nn::socket::FreeAddrInfo(pResult);
    }
    return isFound;
}

// 0x003E6C10 | fefates:bytes [tier B]
void nn::pia::inet::SocketAddress::GetInetAddress(nn::pia::common::InetAddress* pAddress)
{
    if (!common::IsValidPointer(pAddress)) {
        return;
    }
    pAddress->m_Address = SwapU32(m_pSockAddrIn->m_Address);
    pAddress->m_Port = SwapU16(m_pSockAddrIn->m_Port);
}

// 0x003E6C50 | fefates:bytes [tier B]
void nn::pia::inet::SocketAddress::SetInetAddress(const nn::pia::common::InetAddress& address)
{
    m_pSockAddrIn->m_Address = SwapU32(address.m_Address);
    m_pSockAddrIn->m_Port = SwapU16(address.m_Port);
}

// 0x003E6C7C | fefates:bytes [tier B]
void nn::pia::inet::SocketAddress::Init()
{
    m_pSockAddrIn = &m_SockAddrIn;
    memset(&m_SockAddrIn, 0, sizeof(m_SockAddrIn));
    m_pSockAddrIn->m_Family = nn::socket::AF_INET;
    m_pSockAddrIn->m_Length = sizeof(SockAddrIn);
}

} // namespace inet
} // namespace pia
} // namespace nn
