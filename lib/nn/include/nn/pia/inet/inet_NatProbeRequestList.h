#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/inet/inet_NatProbeRequest.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet19NatProbeRequestListE @ 0x008CF918
// vtable 0x00900204 (vptr 0x0090020C), offset_to_top 0, 2 entries
//
// The pending NAT traversal requests (at most REQUEST_NUM_MAX, the nodes in a buffer of its own),
// built like NatProbeList. The member names are ours.
class NatProbeRequestList : public ::nn::pia::common::ObjList<nn::pia::inet::NatProbeRequest>
{
public:
    static const u32 REQUEST_NUM_MAX = 24;

    NatProbeRequestList(); // 0x003F8A70 | fefates:bytes [tier B]
    virtual ~NatProbeRequestList(); // 0x003F8C5C slot 0x00 | fefates:callgraph
    // 0x003F8BA4 slot 0x04 (deleting dtor)

    u8* m_pNodeBuffer; // 0x30
};
ASSERT_OFFSET(NatProbeRequestList, m_pNodeBuffer, 0x30);
ASSERT_SIZE(NatProbeRequestList, 0x34);
} // namespace inet
} // namespace pia
} // namespace nn
