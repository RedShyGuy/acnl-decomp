#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet16SocketStreamBaseE @ 0x008CF8A8
// vtable 0x00900060 (vptr 0x00900068), offset_to_top 0, 2 entries
class SocketStreamBase : public ::nn::pia::common::RootObject
{
public:
    virtual ~SocketStreamBase(); // 0x003E8000 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x003E7FE4 slot 0x04 | virtual slot, introduced by nn::pia::inet::SocketStreamBase
    SocketStreamBase(); // 0x003E7FAC | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
