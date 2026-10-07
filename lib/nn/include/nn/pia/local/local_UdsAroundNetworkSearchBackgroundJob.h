#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchBackgroundJob.h"
#include "nn/pia/local/local_UdsNetworkDescription.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local35UdsAroundNetworkSearchBackgroundJobE @ 0x008CFDD8
// vtable 0x009014B4 (vptr 0x009014BC), offset_to_top 0, 7 entries
//
// The scan of the search of the networks around with uds (uds::CTR::ScanOnConnection); the found
// networks of other sessions become statuses of LocalAroundNetworkSearchManager. The member names
// are ours.
class UdsAroundNetworkSearchBackgroundJob : public ::nn::pia::local::LocalAroundNetworkSearchBackgroundJob
{
public:
    static const u32 NODE_INFORMATION_NUM = 16;

    UdsAroundNetworkSearchBackgroundJob(); // 0x00425BFC
    // (a nop that falls into the destructor of LocalAroundNetworkSearchBackgroundJob)
    virtual ~UdsAroundNetworkSearchBackgroundJob(); // 0x00425DD0 slot 0x00
    // 0x00425C20 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00731804 slot 0x14
    virtual nn::Result ScanNetwork(); // 0x004258C8 slot 0x18

    nn::pia::local::UdsNetworkDescription m_Description;                         // 0x058
    nn::uds::CTR::NodeInformation m_NodeInformations[NODE_INFORMATION_NUM];      // 0x164
};
ASSERT_SIZE(UdsAroundNetworkSearchBackgroundJob, 0x3E8);
} // namespace local
} // namespace pia
} // namespace nn
