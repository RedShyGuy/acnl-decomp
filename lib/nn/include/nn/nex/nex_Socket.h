#pragma once

#include "decomp.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SocketDriver.h"
#include "nn/nex/nex_SocketDriver_Socket.h"

namespace nn {
namespace nex {
// A socket of nex over a socket of the socket driver. The names of the members and of the
// functions without a reference name are ours.
// RTTI N2nn3nex6SocketE @ 0x008CF5C4
// vtable 0x008FFA3C (vptr 0x008FFA44), offset_to_top 0, 6 entries
class Socket : public ::nn::nex::RootObject
{
public:
    // the states of the socket
    enum State : u8
    {
        STATE_OPEN = 0,
        STATE_BOUND = 1,
        STATE_CLOSED = 3,
    };
    // the time to live of the packets
    static const u8 DEFAULT_TTL = 64;
    // the results of the receives and sends
    static const s32 RESULT_SUCCESS = 0;
    static const s32 RESULT_ERROR = -1;

    // (inline)
    Socket(u32 context, u32 maxPacketSize)
        : m_State(STATE_CLOSED), m_IsDatagram(true), m_Context(context), m_MaxPacketSize(maxPacketSize), m_HasLengthPrefix(false), m_IsAvailable(true)
    {
        m_pSocket = SocketDriver::s_pInstance != nullptr ? SocketDriver::s_pInstance->Create() : nullptr;
    }
    virtual ~Socket(); // 0x003D0FF8 slot 0x00
    // 0x003D0F78 slot 0x04 (deleting dtor)
    // binds the socket to the port of the address (the port is updated)
    virtual bool Bind(nn::nex::InetAddress* pAddress, bool unknown); // 0x003D0C08 slot 0x08 | fefates:callseq-callee
    // receives into the buffer, waiting; 0 or -1
    virtual s32 Receive(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x003D0CDC slot 0x0C
    // receives into the buffer, not waiting; 0 or -1
    virtual s32 ReceiveNonBlocking(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x003D0B98 slot 0x10
    // sends the content of the buffer (with its time to live); 0 or -1
    virtual s32 Send(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress); // 0x003D0DE4 slot 0x14

    bool Open(bool isDatagram); // 0x003D0C8C | mk7dlp:bytes [tier A]
    void Close(); // 0x003D0E58 | fefates:bytes [tier B]
    // 0 or -1 (the address of a stream socket is 0)
    s32 RecvFrom(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize, nn::nex::InetAddress* pAddress); // 0x003D0BE8
    DECOMP_NOINLINE s32 RecvFrom(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize, nn::nex::InetAddress* pAddress, nn::nex::SocketDriver::_SocketFlag flags); // 0x003D0E8C
    DECOMP_NOINLINE s32 SendTo(unsigned char* pData, unsigned int size, const nn::nex::InetAddress* pAddress); // 0x003D0D28

    // (inline) Bind, also in QueuingSocket
    bool BindInline(nn::nex::InetAddress* pAddress)
    {
        if (m_pSocket == nullptr) {
            return false;
        }
        u16 port = __builtin_bswap16(pAddress->m_SockAddr.m_Port);
        if (!m_pSocket->Bind(port)) {
            return false;
        }
        pAddress->m_SockAddr.m_Port = __builtin_bswap16(port);
        m_pSocket->SetTTL(DEFAULT_TTL);
        m_State = STATE_BOUND;
        m_Address.m_SockAddr = pAddress->m_SockAddr;
        return true;
    }
    // (inline) ReceiveNonBlocking, also in QueuingSocket
    s32 ReceiveNonBlockingInline(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress);
    // (inline) Send, also in QueuingSocket
    s32 SendInline(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress);
    // (inline) closes the socket of the driver
    void CloseInline()
    {
        if (m_State != STATE_CLOSED && m_pSocket != nullptr) {
            m_pSocket->Close();
            m_State = STATE_CLOSED;
        }
    }

    State m_State;                        // 0x04
    InetAddress m_Address;                // 0x08 (where it is bound)
    u32 m_Unknown0x28;                    // 0x28
    bool m_IsDatagram;                    // 0x2C
    u32 m_Context;                        // 0x30 (given by the creator)
    u32 m_MaxPacketSize;                  // 0x34
    bool m_HasLengthPrefix;               // 0x38 (the bundles start with their length)
    SocketDriver::Socket* m_pSocket;      // 0x3C
    bool m_IsAvailable;                   // 0x40 (false after the connection was closed)
};
ASSERT_OFFSET(Socket, m_pSocket, 0x3C);
ASSERT_SIZE(Socket, 0x44);

inline s32 Socket::ReceiveNonBlockingInline(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    u32 receivedSize = 0;
    s32 result = RecvFrom(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_Size - pBuffer->m_HeaderSize, &receivedSize, pAddress,
                          SocketDriver::SOCKET_FLAG_DONT_WAIT);
    if (result == RESULT_SUCCESS) {
        pBuffer->m_ContentSize = receivedSize;
    }
    return result;
}

inline s32 Socket::SendInline(nn::nex::Buffer* pBuffer, nn::nex::InetAddress* pAddress)
{
    u8 ttl = pBuffer->m_TTL;
    if (ttl != 0) {
        m_pSocket->SetTTL(ttl);
    }
    s32 result = SendTo(pBuffer->m_pData + pBuffer->m_HeaderSize, pBuffer->m_ContentSize, pAddress);
    if (ttl != 0) {
        m_pSocket->SetTTL(DEFAULT_TTL);
    }
    return result;
}
} // namespace nex
} // namespace nn
