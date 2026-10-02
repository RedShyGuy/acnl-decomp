#pragma once

#include "decomp.h"
#include "nn/pia/common/common_IPacketOutput.h"
#include "nn/pia/inet/inet_SocketStreamBase.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet18SocketOutputStreamE @ 0x008CF8F8
// vtable 0x009001BC (vptr 0x009001C4), offset_to_top 0, 8 entries
// vtable 0x009001E4 (vptr 0x009001EC), offset_to_top -1376, 6 entries
class SocketOutputStream : public ::nn::pia::inet::SocketStreamBase, public ::nn::pia::common::IPacketOutput
{
public:
    virtual ~SocketOutputStream(); // 0x003F8A6C slot 0x00 | slot vf_0x00 of nn::pia::inet::SocketStreamBase
    virtual void vf_0x04(); // 0x003F8A5C slot 0x04 | virtual slot, introduced by nn::pia::inet::SocketStreamBase
    virtual void OnStationConnectionEvent(); // 0x003F85F4 slot 0x08 | fefates:bytes
    virtual void vf_0x0C(); // 0x003F8868 slot 0x0C | virtual slot, introduced by nn::pia::inet::SocketOutputStream
    virtual void vf_0x10(); // 0x003F86BC slot 0x10 | fefates:callseq
    virtual void vf_0x14(); // 0x0072F160 slot 0x14 | virtual slot, introduced by nn::pia::inet::SocketOutputStream
    virtual void vf_0x18(); // 0x0072F158 slot 0x18 | virtual slot, introduced by nn::pia::inet::SocketOutputStream
    virtual void vf_0x1C(); // 0x003F8420 slot 0x1C | virtual slot, introduced by nn::pia::inet::SocketOutputStream
    void SendToMulti(const void*, unsigned int, const nn::pia::inet::SockAddrIn*, int); // 0x003F8578 | fefates:bytes [tier B]
    void SendTo(const void*, unsigned int, const nn::pia::common::InetAddress&, unsigned char); // 0x003F894C | fefates:bytes [tier B]
    SocketOutputStream(); // 0x003F8A3C | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
