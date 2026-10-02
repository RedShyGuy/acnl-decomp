#pragma once

#include "decomp.h"
#include "nn/nex/nex_BerkeleySocketDriver.h"
#include "nn/nex/nex_SocketDriver.h"
#include "nn/nex/nex_SocketDriver_Socket.h"

// RTTI N2nn3nex20BerkeleySocketDriver14BerkeleySocketE @ 0x008CE844
// vtable 0x008FD780 (vptr 0x008FD788), offset_to_top 0, 14 entries
class nn::nex::BerkeleySocketDriver::BerkeleySocket : public ::nn::nex::SocketDriver::Socket
{
public:
    BerkeleySocket(); // ctor address unknown
    virtual void Open(nn::nex::SocketDriver::_TrafficType); // 0x003956D4 slot 0x00 | slot vf_0x00 of nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void vf_0x04(); // 0x00395964 slot 0x04 | fefates:callseq
    virtual void Bind(unsigned short&); // 0x003955AC slot 0x08 | fefates:bytes-fuzzy
    virtual void RecvFrom(unsigned char*, unsigned int, nn::nex::SocketDriver::InetAddress*, unsigned int*, nn::nex::SocketDriver::_SocketFlag); // 0x00395C80 slot 0x0C | slot vf_0x0C of nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void SendTo(unsigned char*, unsigned int, const nn::nex::SocketDriver::InetAddress&, unsigned int*); // 0x00395994 slot 0x10 | slot vf_0x10 of nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void vf_0x14(); // 0x00395E90 slot 0x14 | fefates:callseq
    virtual void vf_0x18(); // 0x00395C00 slot 0x18 | virtual slot, introduced by nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void Recv(unsigned char*, unsigned int, unsigned int*); // 0x0039578C slot 0x1C | slot vf_0x1C of nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void vf_0x20(); // 0x00395878 slot 0x20 | fefates:callseq
    virtual void vf_0x24(); // 0x003955A4 slot 0x24 | virtual slot, introduced by nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void vf_0x28(); // 0x00395BB0 slot 0x28 | fefates:callseq
    virtual void vf_0x2C(); // 0x00395EE0 slot 0x2C | virtual slot, introduced by nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual ~BerkeleySocket(); // 0x00395EC8 slot 0x30 | slot vf_0x30 of nn::nex::BerkeleySocketDriver::BerkeleySocket
    virtual void vf_0x34(); // 0x00395DE4 slot 0x34 | virtual slot, introduced by nn::nex::BerkeleySocketDriver::BerkeleySocket
};
