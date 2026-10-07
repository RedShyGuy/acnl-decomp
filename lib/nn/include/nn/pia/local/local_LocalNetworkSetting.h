#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalNetworkSettingE
// vtable 0x00900CAC (vptr 0x00900CB4), offset_to_top 0, 1 entry
//
// The setting of LocalNetwork::Initialize: the optional parts of the local network and the version of
// the application in the beacons. The layout is from LocalNetwork::InitializeCore and the setup code
// of the game; the member names are ours.
class LocalNetworkSetting : public ::nn::pia::common::RootObject
{
public:
    LocalNetworkSetting() : m_IsHostMigrationEnabled(false), m_IsAroundNetworkSearchEnabled(false), m_ApplicationVersion(0) {}
    // true: LocalNetwork::Initialize refuses the setting (UdsNetworkSetting: false; name is ours)
    virtual bool vf_0x00() const = 0; // slot 0x00

    bool m_IsHostMigrationEnabled;       // 0x04, LocalMigrationManager
    bool m_IsAroundNetworkSearchEnabled; // 0x05, LocalAroundNetworkSearchManager
    u8 m_ApplicationVersion;             // 0x06, in the beacon (connections need the same)
};
ASSERT_SIZE(LocalNetworkSetting, 0x8);
} // namespace local
} // namespace pia
} // namespace nn
