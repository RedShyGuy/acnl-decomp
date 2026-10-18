#pragma once

#include "decomp.h"
#include "nn/nex/nex_BerkeleySocketDriver.h"
#include "nn/nex/nex_SocketDriver.h"
#include "nn/nex/nex_SocketDriver_Socket.h"

// A socket of the socket service. The names of the members are ours.
// RTTI N2nn3nex20BerkeleySocketDriver14BerkeleySocketE @ 0x008CE844
// vtable 0x008FD780 (vptr 0x008FD788), offset_to_top 0, 14 entries
class nn::nex::BerkeleySocketDriver::BerkeleySocket : public ::nn::nex::SocketDriver::Socket
{
public:
    // (inline in BerkeleySocketDriver::Create)
    explicit BerkeleySocket(::SendMultiParameter* pSendMultiParameter) : m_IsNonBlocking(false), m_Socket(-1), m_pSendMultiParameter(pSendMultiParameter) {}
    virtual bool Open(nn::nex::SocketDriver::_TrafficType type); // 0x003956D4 slot 0x00
    virtual void Close(); // 0x00395964 slot 0x04 | fefates:callseq
    virtual bool Bind(unsigned short& port); // 0x003955AC slot 0x08 | fefates:bytes-fuzzy
    virtual nn::nex::SocketDriver::_SocketResult RecvFrom(unsigned char* pBuffer, unsigned int size, nn::nex::SocketDriver::InetAddress* pAddress,
                                                          unsigned int* pReceivedSize, nn::nex::SocketDriver::_SocketFlag flags); // 0x00395C80 slot 0x0C
    virtual nn::nex::SocketDriver::_SocketResult SendTo(unsigned char* pBuffer, unsigned int size, const nn::nex::SocketDriver::InetAddress& address,
                                                        unsigned int* pSentSize); // 0x00395994 slot 0x10
    virtual nn::nex::SocketDriver::_SocketResult Shutdown(); // 0x00395E90 slot 0x14 | fefates:callseq
    virtual nn::nex::SocketDriver::_SocketResult Connect(const nn::nex::SocketDriver::InetAddress& address); // 0x00395C00 slot 0x18
    virtual nn::nex::SocketDriver::_SocketResult Recv(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize); // 0x0039578C slot 0x1C
    virtual nn::nex::SocketDriver::_SocketResult Send(unsigned char* pBuffer, unsigned int size, unsigned int* pSentSize); // 0x00395878 slot 0x20 | fefates:callseq
    virtual bool IsAvailable(); // 0x003955A4 slot 0x24
    virtual nn::nex::SocketDriver::_SocketResult SetTTL(unsigned char ttl); // 0x00395BB0 slot 0x28 | fefates:callseq
    virtual ~BerkeleySocket(); // 0x00395EE0 slot 0x2C
    // 0x00395EC8 slot 0x30 (deleting dtor)
    virtual bool SetNonBlocking(bool isNonBlocking); // 0x00395DE4 slot 0x34

    bool m_IsNonBlocking;                       // 0x04
    s32 m_Socket;                               // 0x08 (-1: none)
    ::SendMultiParameter* m_pSendMultiParameter; // 0x0C (of the context of the creation)
};
ASSERT_SIZE(nn::nex::BerkeleySocketDriver::BerkeleySocket, 0x10);
