#include "nn/nex/nex_BerkeleySocketDriver_BerkeleySocket.h"
#include "nn/socket/detail/detail_Api.h"
#include "nn/socket/socket_Types.h"

namespace nn {
namespace nex {
namespace {
// the arguments of the socket service (the names are ours)
const s32 SOCKET_TYPE_STREAM = 1;
const s32 SOCKET_TYPE_DATAGRAM = 2;
const s32 PROTOCOL_TCP = 6;
const s32 LEVEL_IP = 0;
const s32 OPTION_TTL = 8;
const s32 FLAG_DONT_WAIT = 4;
const s32 SHUTDOWN_BOTH = 2;
const s32 FCNTL_GET_FLAGS = 3;
const s32 FCNTL_SET_FLAGS = 4;
const s32 FLAG_NON_BLOCKING = 4;
// the largest size of the "small" calls of the socket service
const u32 SMALL_SIZE_MAX = 8192;
// the error numbers of the socket service
const s32 ERROR_WOULD_BLOCK = -6;
const s32 ERROR_BAD_SOCKET = -8;
const s32 ERROR_IN_PROGRESS = -26;
const s32 ERROR_CONNECTION_RESET = -38;
// the ports that Bind tries for port 0
const u16 FIRST_PORT = 9104;
const u16 LAST_PORT = FIRST_PORT + 96;

typedef ::SendMultiParameter::Address SockAddr;

// (inline) the return value of a call, or the error number of the IPC result
inline s32 ToReturnValue(nn::Result result, s32 value)
{
    return result.IsSuccess() ? value : nn::socket::detail::ConvertErrorResult(result);
}

// (inline) an IPv4 address of the socket service
inline void MakeSockAddr(SockAddr* pAddress, u32 address, u16 port)
{
    *reinterpret_cast<u32*>(pAddress) = 0;
    pAddress->m_Family = nn::socket::AF_INET;
    pAddress->m_Address = 0;
    pAddress->m_Length = sizeof(SockAddr);
    pAddress->m_Address = __builtin_bswap32(address);
    pAddress->m_Port = __builtin_bswap16(port);
}

// (inline) the result of a negative return value (a reset connection closes the socket)
inline SocketDriver::_SocketResult ToSocketResult(SocketDriver::Socket* pSocket, s32 error)
{
    if (error == ERROR_CONNECTION_RESET) {
        pSocket->Shutdown();
        pSocket->Close();
        return SocketDriver::SOCKET_RESULT_CLOSED;
    }
    if (error == ERROR_BAD_SOCKET) {
        return SocketDriver::SOCKET_RESULT_CLOSED;
    }
    return error == ERROR_WOULD_BLOCK ? SocketDriver::SOCKET_RESULT_WOULD_BLOCK : SocketDriver::SOCKET_RESULT_ERROR;
}
} // namespace

// 0x003955A4 slot 0x24
bool nn::nex::BerkeleySocketDriver::BerkeleySocket::IsAvailable()
{
    return true;
}

// 0x003955AC slot 0x08 | fefates:bytes-fuzzy
bool nn::nex::BerkeleySocketDriver::BerkeleySocket::Bind(unsigned short& port)
{
    if (port == 0) {
        // the first free port of the range
        for (port = FIRST_PORT; port <= LAST_PORT; port++) {
            if (Bind(port)) {
                return true;
            }
        }
        port = 0;
        return false;
    }
    SockAddr address;
    MakeSockAddr(&address, 0, port);
    s32 value = 0;
    if (ToReturnValue(nn::socket::detail::Bind(&value, m_Socket, reinterpret_cast<const u8*>(&address), sizeof(address)), value) != 0) {
        return false;
    }
    if (port == 0) {
        value = 0;
        if (ToReturnValue(nn::socket::detail::GetSockName(&value, m_Socket, reinterpret_cast<u8*>(&address), sizeof(address)), value) < 0) {
            return false;
        }
        port = __builtin_bswap16(address.m_Port);
    }
    return true;
}

// 0x003956D4 slot 0x00
bool nn::nex::BerkeleySocketDriver::BerkeleySocket::Open(nn::nex::SocketDriver::_TrafficType type)
{
    s32 value = 0;
    if (type == TRAFFIC_TYPE_DATAGRAM) {
        m_Socket = ToReturnValue(nn::socket::detail::Socket(&value, nn::socket::AF_INET, SOCKET_TYPE_DATAGRAM, 0), value);
    } else if (type == TRAFFIC_TYPE_STREAM) {
        m_Socket = ToReturnValue(nn::socket::detail::Socket(&value, nn::socket::AF_INET, SOCKET_TYPE_STREAM, PROTOCOL_TCP), value);
    }
    if (m_Socket < 0) {
        return false;
    }
    SetNonBlocking(false);
    return true;
}

// 0x0039578C slot 0x1C
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::Recv(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize)
{
    s32 value = 0;
    nn::Result result = size > SMALL_SIZE_MAX ? nn::socket::detail::RecvFrom(&value, m_Socket, pBuffer, size, FLAG_DONT_WAIT, nullptr, sizeof(SockAddr))
                                              : nn::socket::detail::RecvFromSmall(&value, m_Socket, pBuffer, size, FLAG_DONT_WAIT, nullptr, sizeof(SockAddr));
    s32 received = ToReturnValue(result, value);
    _SocketResult socketResult = SOCKET_RESULT_SUCCESS;
    if (received < 0) {
        socketResult = ToSocketResult(this, received);
    } else if (pReceivedSize != nullptr) {
        *pReceivedSize = received;
    }
    s_RecvHook(socketResult, pBuffer, size, pReceivedSize);
    return socketResult;
}

// 0x00395878 slot 0x20 | fefates:callseq
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::Send(unsigned char* pBuffer, unsigned int size, unsigned int* pSentSize)
{
    s32 value = 0;
    nn::Result result = size > SMALL_SIZE_MAX ? nn::socket::detail::SendTo(&value, m_Socket, pBuffer, size, FLAG_DONT_WAIT, nullptr, sizeof(SockAddr))
                                              : nn::socket::detail::SendToSmall(&value, m_Socket, pBuffer, size, FLAG_DONT_WAIT, nullptr, sizeof(SockAddr));
    s32 sent = ToReturnValue(result, value);
    _SocketResult socketResult = SOCKET_RESULT_SUCCESS;
    if (sent < 0) {
        socketResult = ToSocketResult(this, sent);
    } else if (pSentSize != nullptr) {
        *pSentSize = sent;
    }
    s_SendHook(socketResult, pBuffer, size, pSentSize);
    return socketResult;
}

// 0x00395964 slot 0x04 | fefates:callseq
void nn::nex::BerkeleySocketDriver::BerkeleySocket::Close()
{
    s32 value = 0;
    ToReturnValue(nn::socket::detail::Close(&value, m_Socket), value);
    m_Socket = -1;
}

// 0x00395994 slot 0x10
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::SendTo(unsigned char* pBuffer, unsigned int size,
                                                                                         const nn::nex::SocketDriver::InetAddress& address, unsigned int* pSentSize)
{
    s32 value = 0;
    nn::Result result;
    if (address.m_Port != 0) {
        s_Counters.m_SendToCount++;
        SockAddr sockAddr;
        MakeSockAddr(&sockAddr, address.m_Address, address.m_Port);
        result = size > SMALL_SIZE_MAX
                     ? nn::socket::detail::SendTo(&value, m_Socket, pBuffer, size, 0, reinterpret_cast<const u8*>(&sockAddr), sizeof(sockAddr))
                     : nn::socket::detail::SendToSmall(&value, m_Socket, pBuffer, size, 0, reinterpret_cast<const u8*>(&sockAddr), sizeof(sockAddr));
    } else {
        // port 0: to all the addresses of the parameter (starting at another one each time)
        s_Counters.m_SendMultiCount++;
        SockAddr addresses[::SendMultiParameter::MAX_ADDRESSES];
        u32 count = 0;
        ::SendMultiParameter* pParameter = m_pSendMultiParameter;
        if (pParameter->m_Count <= ::SendMultiParameter::MAX_ADDRESSES) {
            ScopedCS lock(pParameter->m_Lock);
            u32 first = pParameter->m_Index++;
            for (u32 i = 0; i < pParameter->m_Count; i++) {
                addresses[i] = pParameter->m_Addresses[(first + i) % pParameter->m_Count];
            }
            count = pParameter->m_Count;
        }
        result = nn::socket::detail::SendToSmallMulti(&value, m_Socket, pBuffer, size, 0, reinterpret_cast<const u8*>(addresses), sizeof(SockAddr),
                                                      count * sizeof(SockAddr));
    }
    s32 sent = ToReturnValue(result, value);
    _SocketResult socketResult;
    if (sent < 0) {
        socketResult = ToSocketResult(this, sent);
    } else {
        socketResult = SOCKET_RESULT_SUCCESS;
        if (pSentSize != nullptr) {
            *pSentSize = sent;
        }
    }
    s_SendToHook(socketResult, pBuffer, size, &address, pSentSize);
    return socketResult;
}

// 0x00395BB0 slot 0x28 | fefates:callseq
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::SetTTL(unsigned char ttl)
{
    s32 option = ttl;
    s32 value = 0;
    if (ToReturnValue(nn::socket::detail::SetSockOpt(&value, m_Socket, LEVEL_IP, OPTION_TTL, reinterpret_cast<const u8*>(&option), sizeof(option)), value) != 0) {
        return SOCKET_RESULT_ERROR;
    }
    return SOCKET_RESULT_SUCCESS;
}

// 0x00395C00 slot 0x18
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::Connect(const nn::nex::SocketDriver::InetAddress& address)
{
    SockAddr sockAddr;
    MakeSockAddr(&sockAddr, address.m_Address, address.m_Port);
    s32 value = 0;
    s32 error = ToReturnValue(nn::socket::detail::Connect(&value, m_Socket, reinterpret_cast<const u8*>(&sockAddr), sizeof(sockAddr)), value);
    if (error >= 0) {
        return SOCKET_RESULT_SUCCESS;
    }
    return error == ERROR_IN_PROGRESS ? SOCKET_RESULT_IN_PROGRESS : SOCKET_RESULT_ERROR;
}

// 0x00395C80 slot 0x0C
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::RecvFrom(unsigned char* pBuffer, unsigned int size,
                                                                                           nn::nex::SocketDriver::InetAddress* pAddress, unsigned int* pReceivedSize,
                                                                                           nn::nex::SocketDriver::_SocketFlag flags)
{
    SockAddr sockAddr;
    MakeSockAddr(&sockAddr, 0, 0);
    s_Counters.m_RecvFromCount++;
    s32 recvFlags = (flags & SOCKET_FLAG_DONT_WAIT) ? FLAG_DONT_WAIT : 0;
    s32 value = 0;
    nn::Result result = size > SMALL_SIZE_MAX
                            ? nn::socket::detail::RecvFrom(&value, m_Socket, pBuffer, size, recvFlags, reinterpret_cast<u8*>(&sockAddr), sizeof(sockAddr))
                            : nn::socket::detail::RecvFromSmall(&value, m_Socket, pBuffer, size, recvFlags, reinterpret_cast<u8*>(&sockAddr), sizeof(sockAddr));
    s32 received = ToReturnValue(result, value);
    _SocketResult socketResult;
    if (received < 0) {
        socketResult = ToSocketResult(this, received);
    } else {
        socketResult = SOCKET_RESULT_SUCCESS;
        s_Counters.m_ReceivedCount++;
        if (pAddress != nullptr) {
            pAddress->m_Address = __builtin_bswap32(sockAddr.m_Address);
            pAddress->m_Port = __builtin_bswap16(sockAddr.m_Port);
        }
        if (pReceivedSize != nullptr) {
            *pReceivedSize = received;
        }
    }
    s_RecvFromHook(socketResult, pBuffer, size, pAddress, pReceivedSize);
    return socketResult;
}

// 0x00395DE4 slot 0x34
bool nn::nex::BerkeleySocketDriver::BerkeleySocket::SetNonBlocking(bool isNonBlocking)
{
    s32 value = 0;
    s32 flags = ToReturnValue(nn::socket::detail::Fcntl(&value, m_Socket, FCNTL_GET_FLAGS, 0), value);
    value = 0;
    flags = isNonBlocking ? flags | FLAG_NON_BLOCKING : flags & ~FLAG_NON_BLOCKING;
    if (ToReturnValue(nn::socket::detail::Fcntl(&value, m_Socket, FCNTL_SET_FLAGS, flags), value) < 0) {
        return false;
    }
    m_IsNonBlocking = isNonBlocking;
    return true;
}

// 0x00395E90 slot 0x14 | fefates:callseq
nn::nex::SocketDriver::_SocketResult nn::nex::BerkeleySocketDriver::BerkeleySocket::Shutdown()
{
    s32 value = 0;
    if (ToReturnValue(nn::socket::detail::Shutdown(&value, m_Socket, SHUTDOWN_BOTH), value) < 0) {
        return SOCKET_RESULT_ERROR;
    }
    return SOCKET_RESULT_SUCCESS;
}

// 0x00395EE0 slot 0x2C
// 0x00395EC8 (deleting dtor)
nn::nex::BerkeleySocketDriver::BerkeleySocket::~BerkeleySocket()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
