#include "nn/pia/inet/inet_Socket.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_Api.h"
#include "nn/pia/inet/inet_SocketAddress.h"
#include "nn/socket/detail/detail_Api.h"
#include "nn/socket/socket_Types.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
// the socket type of Open() (a datagram socket of the socket service)
const int SOCKET_TYPE_DATAGRAM = 2;
// SetTtl: the option level and name
const int SOCKET_LEVEL_IP = 0;
const int SOCKET_OPTION_TTL = 8;
// RecvFrom does not block
const int RECV_FLAG_DONT_WAIT = 4;
// the largest size the "small" functions of the socket service take
const int SMALL_SIZE_MAX = 8192;

// The calls of the socket service as the library wraps them: the return value, or the error
// number of the IPC result (inline; the names are ours).
inline s32 ToReturnValue(nn::Result result, s32 value)
{
    return result.IsSuccess() ? value : nn::socket::detail::ConvertErrorResult(result);
}

// the result of a negative return value of the socket service (inline; name is ours, after
// convertSendSocketErrorToResult); the numbers are the error numbers of the socket service
inline nn::Result convertSocketErrorToResult(int error)
{
    if (error == -15 || error == -38 || error == -56 || error == -76) {
        return common::RESULT_SOCKET_UNAVAILABLE;
    }
    if (error == -8 || error == -28 || error == -68 || error == -5 || error == -69 || error == -63 || error == -4 || error == -51) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (error == -49 || error == -42 || error == -33 || error == -39) {
        return common::RESULT_INVALID_STATE;
    }
    if (error == -3) {
        return common::RESULT_SOCKET_ERROR_194;
    }
    if (error == -10) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    return common::RESULT_INVALID_STATE;
}
} // namespace

// 0x00411504 | fefates:bytes [tier B]
nn::Result convertSendSocketErrorToResult(int error)
{
    if (error == -15 || error == -38 || error == -56 || error == -76 || error == -14 || error == -40) {
        return common::RESULT_SOCKET_UNAVAILABLE;
    }
    if (error == -8 || error == -28 || error == -63 || error == -17 || error == -35 || error == -69 || error == -5) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (error == -39) {
        return common::RESULT_INVALID_STATE;
    }
    if (error == -6 || error == -10 || error == -27 || error == -49 || error == -42) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    return common::RESULT_INVALID_STATE;
}

// 0x0041243C | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::SendToMulti(const void* pData, int size, const nn::pia::inet::SockAddrIn* pAddresses, int addressNum, int* pSentSize)
{
    if (m_Descriptor < 0 || addressNum <= 0 || addressNum > SEND_ADDRESS_NUM_MAX) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    s32 value = 0;
    nn::Result result = nn::socket::detail::SendToSmallMulti(&value, m_Descriptor, static_cast<const u8*>(pData), size, 0,
                                                             reinterpret_cast<const u8*>(pAddresses), sizeof(SockAddrIn), addressNum * sizeof(SockAddrIn));
    s32 sentSize = ToReturnValue(result, value);
    if (sentSize < 0) {
        // (the address is taken for a removed trace call)
        common::InetAddress address;
        SocketAddress socketAddress;
        socketAddress.SetSockAddrIn(*pAddresses);
        socketAddress.GetInetAddress(&address);
        return convertSendSocketErrorToResult(sentSize);
    }
    if (pSentSize != nullptr) {
        *pSentSize = sentSize;
    }
    return nn::Result();
}

// 0x00412524 | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::Bind(const nn::pia::common::InetAddress& address)
{
    s32 descriptor = m_Descriptor;
    if (descriptor < 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    m_BoundAddress = address;
    SocketAddress socketAddress;
    socketAddress.SetInetAddress(address);
    s32 value = 0;
    nn::Result result = nn::socket::detail::Bind(&value, descriptor, reinterpret_cast<const u8*>(socketAddress.m_pSockAddrIn), sizeof(SockAddrIn));
    s32 error = ToReturnValue(result, value);
    if (error >= 0) {
        return nn::Result();
    }
    return convertSocketErrorToResult(error);
}

// 0x00412650 | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::Bind(unsigned short port)
{
    common::InetAddress address;
    address.m_Address = 0;
    address.m_Port = port;
    return Bind(address);
}

// 0x00412698 (name is ours)
nn::Result nn::pia::inet::Socket::Open()
{
    return Open(SOCKET_TYPE_DATAGRAM, 0);
}

// 0x004126A4 | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::Open(int type, int protocol)
{
    s32 value = 0;
    nn::Result result = nn::socket::detail::Socket(&value, nn::socket::AF_INET, type, protocol);
    s32 descriptor = ToReturnValue(result, value);
    if (descriptor < 0) {
        return convertSocketErrorToResult(descriptor);
    }
    m_OpenNum++;
    m_Descriptor = descriptor;
    return nn::Result();
}

// 0x004127A0 | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::Close()
{
    if (m_Descriptor < 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    s32 value = 0;
    nn::Result result = nn::socket::detail::Close(&value, m_Descriptor);
    s32 error = ToReturnValue(result, value);
    m_Descriptor = -1;
    m_OpenNum--;
    if (error >= 0) {
        return nn::Result();
    }
    return convertSocketErrorToResult(error);
}

// 0x00412898 | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::SendTo(const void* pData, int size, const nn::pia::common::InetAddress& address, int* pSentSize)
{
    SocketAddress socketAddress;
    socketAddress.SetInetAddress(address);
    s32 descriptor = m_Descriptor;
    if (descriptor < 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    s32 value = 0;
    nn::Result result;
    if (size <= SMALL_SIZE_MAX) {
        result = nn::socket::detail::SendToSmall(&value, descriptor, static_cast<const u8*>(pData), size, 0,
                                                 reinterpret_cast<const u8*>(socketAddress.m_pSockAddrIn), sizeof(SockAddrIn));
    } else {
        result = nn::socket::detail::SendTo(&value, descriptor, static_cast<const u8*>(pData), size, 0,
                                            reinterpret_cast<const u8*>(socketAddress.m_pSockAddrIn), sizeof(SockAddrIn));
    }
    s32 sentSize = ToReturnValue(result, value);
    if (sentSize < 0) {
        return convertSendSocketErrorToResult(sentSize);
    }
    if (pSentSize != nullptr) {
        *pSentSize = sentSize;
    }
    return nn::Result();
}

// 0x0041295C | fefates:bytes [tier B]
nn::Result nn::pia::inet::Socket::SetTtl(unsigned char ttl)
{
    s32 descriptor = m_Descriptor;
    if (descriptor < 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    s32 value = 0;
    int option = ttl;
    nn::Result result = nn::socket::detail::SetSockOpt(&value, descriptor, SOCKET_LEVEL_IP, SOCKET_OPTION_TTL, reinterpret_cast<const u8*>(&option), sizeof(option));
    s32 error = ToReturnValue(result, value);
    if (error >= 0) {
        return nn::Result();
    }
    return convertSocketErrorToResult(error);
}

// 0x00412A6C | fefates:callgraph [tier C]
nn::Result nn::pia::inet::Socket::RecvFrom(unsigned char* pBuffer, int size, nn::pia::common::InetAddress* pAddress, unsigned char*, int* pReceivedSize)
{
    s32 descriptor = m_Descriptor;
    if (descriptor < 0) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    SocketAddress socketAddress;
    s32 value = 0;
    nn::Result result;
    if (size <= SMALL_SIZE_MAX) {
        result = nn::socket::detail::RecvFromSmall(&value, descriptor, pBuffer, size, RECV_FLAG_DONT_WAIT,
                                                   reinterpret_cast<u8*>(socketAddress.m_pSockAddrIn), sizeof(SockAddrIn));
    } else {
        result = nn::socket::detail::RecvFrom(&value, descriptor, pBuffer, size, RECV_FLAG_DONT_WAIT,
                                              reinterpret_cast<u8*>(socketAddress.m_pSockAddrIn), sizeof(SockAddrIn));
    }
    s32 receivedSize = ToReturnValue(result, value);
    if (receivedSize < 0) {
        int error = receivedSize;
        if (error == -8) {
            // the socket was closed meanwhile: nothing to receive
            if (m_Descriptor != descriptor) {
                return common::RESULT_NO_DATA;
            }
            return common::RESULT_INVALID_ARGUMENT;
        }
        if (error == -15 || error == -38 || error == -56 || error == -76) {
            return common::RESULT_SOCKET_UNAVAILABLE;
        }
        if (error == -28 || error == -63 || error == -35 || error == -5) {
            return common::RESULT_INVALID_ARGUMENT;
        }
        if (error == -39) {
            return common::RESULT_INVALID_STATE;
        }
        if (error == -6 || error == -10 || error == -49 || error == -27) {
            return common::RESULT_NO_DATA;
        }
        return common::RESULT_INVALID_STATE;
    }
    if (pAddress != nullptr) {
        socketAddress.GetInetAddress(pAddress);
    }
    if (pReceivedSize != nullptr) {
        *pReceivedSize = receivedSize;
    }
    return nn::Result();
}

// 0x00412BCC | fefates:bytes [tier B]
nn::pia::inet::Socket::Socket() : m_Descriptor(-1), m_OpenNum(0), m_BoundAddress()
{
    // (cleared once more)
    m_BoundAddress.m_Address = 0;
    m_BoundAddress.m_Port = 0;
}

// 0x00412BF4 | fefates:bytes [tier B]
nn::pia::inet::Socket::~Socket()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
