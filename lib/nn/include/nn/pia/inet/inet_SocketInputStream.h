#pragma once

#include "decomp.h"
#include "nn/pia/common/common_IPacketInput.h"
#include "nn/pia/inet/inet_SocketStreamBase.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet17SocketInputStreamE @ 0x008CF8CC
// vtable 0x00900124 (vptr 0x0090012C), offset_to_top 0, 4 entries
// vtable 0x0090013C (vptr 0x00900144), offset_to_top -1376, 3 entries
class SocketInputStream : public ::nn::pia::inet::SocketStreamBase, public ::nn::pia::common::IPacketInput
{
public:
    virtual ~SocketInputStream(); // 0x003E7FFC slot 0x00 | slot vf_0x00 of nn::pia::inet::SocketStreamBase
    virtual void vf_0x04(); // 0x003E8900 slot 0x04 | virtual slot, introduced by nn::pia::inet::SocketStreamBase
    virtual void vf_0x08(); // 0x003E88C4 slot 0x08 | virtual slot, introduced by nn::pia::inet::SocketInputStream
    virtual void vf_0x0C(); // 0x003E88A8 slot 0x0C | virtual slot, introduced by nn::pia::inet::SocketInputStream
    SocketInputStream(); // 0x003E88E0 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
