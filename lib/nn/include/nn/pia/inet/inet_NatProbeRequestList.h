#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/inet/inet_NatProbeRequest.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet19NatProbeRequestListE @ 0x008CF918
// vtable 0x00900204 (vptr 0x0090020C), offset_to_top 0, 2 entries
class NatProbeRequestList : public ::nn::pia::common::ObjList<nn::pia::inet::NatProbeRequest>
{
public:
    virtual void vf_0x00(); // 0x003F8C5C slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x003F8BA4 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeRequestList
    NatProbeRequestList(); // 0x003F8A70 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
