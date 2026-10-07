#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalCreateSessionSetting.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local23UdsCreateSessionSettingE @ 0x008CFC7C
// vtable 0x00901050 (vptr 0x00901058), offset_to_top 0, 5 entries
//
// The setting of a new session of the uds network: the setting of the network to create. The
// member name is ours.
class UdsCreateSessionSetting : public ::nn::pia::local::LocalCreateSessionSetting
{
public:
    UdsCreateSessionSetting(); // 0x0041F774
    // (a nop that falls into the destructor of LocalCreateSessionSetting)
    virtual ~UdsCreateSessionSetting(); // 0x004202FC slot 0x00
    // 0x0041F7D0 slot 0x04 (deleting dtor)
    virtual nn::pia::local::LocalCreateNetworkSetting* GetCreateNetworkSetting(); // 0x0073168C slot 0x08
    virtual void SetCreateNetworkSetting(const nn::pia::local::LocalCreateNetworkSetting& setting); // 0x0041F70C slot 0x0C
    virtual void Trace(u64 flag) const; // 0x00731694 slot 0x10

    nn::pia::local::LocalCreateNetworkSetting m_CreateNetworkSetting; // 0x2C
};
ASSERT_SIZE(UdsCreateSessionSetting, 0x208);
} // namespace local
} // namespace pia
} // namespace nn
