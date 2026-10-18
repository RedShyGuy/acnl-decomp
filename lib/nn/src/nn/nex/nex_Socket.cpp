#include "nn/nex/nex_Socket.h"
#include "nn/nex/nex_Buffer.h"

namespace nn {
namespace nex {
// 0x003D0B98 slot 0x10
s32 nn::nex::Socket::ReceiveNonBlocking(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    return ReceiveNonBlockingInline(pBuffer, pAddress);
}

// 0x003D0BE8 (symbols.json names it nn::friends::CTR::detail::GetFriendScreenName)
s32 nn::nex::Socket::RecvFrom(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize, nn::nex::InetAddress* pAddress)
{
    return RecvFrom(pBuffer, size, pReceivedSize, pAddress, SocketDriver::SOCKET_FLAG_DONT_WAIT);
}

// 0x003D0C08 slot 0x08 | fefates:callseq-callee
bool nn::nex::Socket::Bind(nn::nex::InetAddress* pAddress, bool unknown)
{
    return BindInline(pAddress);
}

// 0x003D0C8C | mk7dlp:bytes [tier A]
bool nn::nex::Socket::Open(bool isDatagram)
{
    m_IsDatagram = isDatagram;
    if (m_pSocket == nullptr) {
        return false;
    }
    if (!m_pSocket->Open(isDatagram ? SocketDriver::TRAFFIC_TYPE_DATAGRAM : SocketDriver::TRAFFIC_TYPE_STREAM)) {
        return false;
    }
    m_State = STATE_OPEN;
    return true;
}

// 0x003D0CDC slot 0x0C
s32 nn::nex::Socket::Receive(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    u32 receivedSize = 0;
    s32 result = RecvFrom(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_Size - pBuffer->m_HeaderSize, &receivedSize, pAddress,
                          SocketDriver::SOCKET_FLAG_NONE);
    if (result == RESULT_SUCCESS) {
        pBuffer->m_ContentSize = receivedSize;
    }
    return result;
}

// 0x003D0D28
s32 nn::nex::Socket::SendTo(unsigned char* pData, unsigned int size, const nn::nex::InetAddress* pAddress)
{
    if (!m_IsAvailable || m_pSocket == nullptr) {
        return RESULT_ERROR;
    }
    u32 sentSize = 0;
    SocketDriver::_SocketResult result;
    if (m_IsDatagram) {
        SocketDriver::InetAddress address;
        address.m_Address = __builtin_bswap32(pAddress->m_SockAddr.m_Address);
        address.m_Port = __builtin_bswap16(pAddress->m_SockAddr.m_Port);
        result = m_pSocket->SendTo(pData, size, address, &sentSize);
    } else {
        result = m_pSocket->Send(pData, size, &sentSize);
    }
    switch (result) {
    case SocketDriver::SOCKET_RESULT_SUCCESS:
        return RESULT_SUCCESS;
    case SocketDriver::SOCKET_RESULT_CLOSED:
        if (m_IsAvailable) {
            m_IsAvailable = false;
        }
        return RESULT_ERROR;
    default:
        return RESULT_ERROR;
    }
}

// 0x003D0DE4 slot 0x14
s32 nn::nex::Socket::Send(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    return SendInline(pBuffer, pAddress);
}

// 0x003D0E58 | fefates:bytes [tier B]
void nn::nex::Socket::Close()
{
    CloseInline();
}

// 0x003D0E8C (symbols.json names it nn::friends::CTR::detail::GetFriendScreenNameEx)
s32 nn::nex::Socket::RecvFrom(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize, nn::nex::InetAddress* pAddress,
                              nn::nex::SocketDriver::_SocketFlag flags)
{
    if (m_pSocket == nullptr) {
        return RESULT_ERROR;
    }
    u32 receivedSize = 0;
    SocketDriver::_SocketResult result;
    if (m_IsDatagram) {
        SocketDriver::InetAddress address;
        address.m_Address = 0;
        address.m_Port = 0;
        result = m_pSocket->RecvFrom(pBuffer, size, &address, &receivedSize, flags);
        pAddress->m_SockAddr.m_Address = __builtin_bswap32(address.m_Address);
        pAddress->m_SockAddr.m_Port = __builtin_bswap16(address.m_Port);
    } else {
        result = m_pSocket->Recv(pBuffer, size, &receivedSize);
        pAddress->m_SockAddr.m_Address = __builtin_bswap32(0);
        pAddress->m_SockAddr.m_Port = __builtin_bswap16(0);
    }
    switch (result) {
    case SocketDriver::SOCKET_RESULT_SUCCESS:
        *pReceivedSize = receivedSize;
        return RESULT_SUCCESS;
    case SocketDriver::SOCKET_RESULT_CLOSED:
        if (m_IsAvailable) {
            m_IsAvailable = false;
        }
        return RESULT_ERROR;
    default:
        return RESULT_ERROR;
    }
}

// 0x003D0FF8 slot 0x00
// 0x003D0F78 (deleting dtor)
nn::nex::Socket::~Socket()
{
    CloseInline();
    if (SocketDriver::s_pInstance != nullptr) {
        SocketDriver::s_pInstance->Delete(m_pSocket);
    }
}

} // namespace nex
} // namespace nn
