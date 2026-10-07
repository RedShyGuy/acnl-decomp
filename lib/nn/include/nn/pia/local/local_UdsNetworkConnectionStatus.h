#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalConnectionStatus.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local26UdsNetworkConnectionStatusE
// vtable 0x00901208 (vptr 0x00901210), offset_to_top 0, 1 entry
//
// The connection status of uds (uds::CTR::GetConnectionStatus). The member name is ours.
class UdsNetworkConnectionStatus : public ::nn::pia::local::LocalConnectionStatus
{
public:
    UdsNetworkConnectionStatus() {}
    virtual u8 GetNodeCount() const; // 0x007316B8 slot 0x00 (name is ours)

    nn::uds::CTR::ConnectionStatus m_Status; // 0x04
};
ASSERT_SIZE(UdsNetworkConnectionStatus, 0x34);
} // namespace local
} // namespace pia
} // namespace nn
