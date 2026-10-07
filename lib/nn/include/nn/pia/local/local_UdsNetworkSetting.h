#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalNetworkSetting.h"

namespace nn {
namespace cfg {
namespace CTR {
struct UserName;
} // namespace CTR
} // namespace cfg
namespace pia {
namespace local {
// RTTI N2nn3pia5local17UdsNetworkSettingE
// vtable 0x00900B4C (vptr 0x00900B54), offset_to_top 0, 1 entry
//
// The setting of the uds network: the buffers of uds::CTR::Initialize and of the scans and the
// options of SendTo / ReceiveFrom. The layout is from LocalNetwork::InitializeCore (which keeps a
// copy) and the setup code of the game; the member names are ours.
class UdsNetworkSetting : public ::nn::pia::local::LocalNetworkSetting
{
public:
    UdsNetworkSetting() : m_ReceiveBufferSize(0), m_pReceiveBuffer(nullptr), m_ScanBufferSize(0), m_SendOption(0), m_ReceiveOption(0), m_pUserName(nullptr) {}
    virtual bool vf_0x00() const; // 0x00730E70 slot 0x00

    u32 m_ReceiveBufferSize;                    // 0x08, at least 16 KB, a multiple of 4 KB
    void* m_pReceiveBuffer;                     // 0x0C, 4 KB aligned; null: on the pia heap
    u32 m_ScanBufferSize;                       // 0x10, at least 1 KB
    u8 m_SendOption;                            // 0x14, of uds::CTR::SendTo
    u8 m_ReceiveOption;                         // 0x15, of uds::CTR::ReceiveFrom (bit 0 set)
    const nn::cfg::CTR::UserName* m_pUserName; // 0x18, of uds::CTR::Initialize (24 bytes copied)
};
ASSERT_SIZE(UdsNetworkSetting, 0x1C);
} // namespace local
} // namespace pia
} // namespace nn
