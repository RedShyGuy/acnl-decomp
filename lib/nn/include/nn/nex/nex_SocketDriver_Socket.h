#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SocketDriver.h"

// A socket of a SocketDriver (the order of the slots is from BerkeleySocket; the names of the slots
// without a reference symbol are ours).
// RTTI N2nn3nex12SocketDriver6SocketE @ 0x008CE1AC
class nn::nex::SocketDriver::Socket : public ::nn::nex::RootObject
{
public:
    // (inline)
    Socket() {}
    virtual bool Open(nn::nex::SocketDriver::_TrafficType type) = 0; // slot 0x00
    virtual void Close() = 0; // slot 0x04
    // port 0: a free port of the range of nex (the bound port is returned)
    virtual bool Bind(unsigned short& port) = 0; // slot 0x08
    virtual nn::nex::SocketDriver::_SocketResult RecvFrom(unsigned char* pBuffer, unsigned int size, nn::nex::SocketDriver::InetAddress* pAddress,
                                                          unsigned int* pReceivedSize, nn::nex::SocketDriver::_SocketFlag flags) = 0; // slot 0x0C
    virtual nn::nex::SocketDriver::_SocketResult SendTo(unsigned char* pBuffer, unsigned int size, const nn::nex::SocketDriver::InetAddress& address,
                                                        unsigned int* pSentSize) = 0; // slot 0x10
    virtual nn::nex::SocketDriver::_SocketResult Shutdown() = 0; // slot 0x14
    virtual nn::nex::SocketDriver::_SocketResult Connect(const nn::nex::SocketDriver::InetAddress& address) = 0; // slot 0x18
    virtual nn::nex::SocketDriver::_SocketResult Recv(unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize) = 0; // slot 0x1C
    virtual nn::nex::SocketDriver::_SocketResult Send(unsigned char* pBuffer, unsigned int size, unsigned int* pSentSize) = 0; // slot 0x20
    virtual bool IsAvailable() = 0; // slot 0x24
    virtual nn::nex::SocketDriver::_SocketResult SetTTL(unsigned char ttl) = 0; // slot 0x28
    virtual ~Socket() {} // slot 0x2C
    // slot 0x30 (deleting dtor)
    virtual bool SetNonBlocking(bool isNonBlocking) = 0; // slot 0x34
};
