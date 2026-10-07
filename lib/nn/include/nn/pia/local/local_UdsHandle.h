#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local9UdsHandleE @ 0x008CFDF0
// vtable 0x009014FC (vptr 0x00901504), offset_to_top 0, 2 entries
//
// The two endpoints of uds that UdsNetworkManager sends and receives with. The layout is from the
// functions; the member names are ours.
class UdsHandle : public ::nn::pia::common::RootObject
{
public:
    // the receive buffer of the attached endpoint
    static const u32 RECEIVE_BUFFER_SIZE = 0x2E30;

    UdsHandle(); // 0x00425F64 (symbols.json: nn::nex::UdsHandle, wrong)
    virtual ~UdsHandle(); // 0x00425F84 slot 0x00
    // 0x00425F80 slot 0x04 (deleting dtor)

    nn::Result CreateHandle(u16 dataChannel); // 0x00425E80 | fefates:bytes [tier B]
    nn::Result DestroyHandle(); // 0x00425F18 | fefates:bytes [tier B]

    nn::uds::CTR::EndpointDescriptor m_SendEndpoint;    // 0x04
    nn::uds::CTR::EndpointDescriptor m_ReceiveEndpoint; // 0x08
    u16 m_DataChannel;                                  // 0x0C
    bool m_IsCreated;                                   // 0x0E
};
ASSERT_SIZE(UdsHandle, 0x10);
} // namespace local
} // namespace pia
} // namespace nn
