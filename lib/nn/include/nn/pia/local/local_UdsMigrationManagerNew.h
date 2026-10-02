#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMigrationManager.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local22UdsMigrationManagerNewE @ 0x008CFC4C
// vtable 0x00900FC0 (vptr 0x00900FC8), offset_to_top 0, 11 entries
class UdsMigrationManagerNew : public ::nn::pia::local::LocalMigrationManager
{
public:
    virtual ~UdsMigrationManagerNew(); // 0x0041E908 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x0041E89C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMigrationManager
    virtual void SetupParams(); // 0x0041E4F4 slot 0x08 | fefates:bytes
    virtual void SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting&); // 0x0041E5D0 slot 0x0C | fefates:bytes
    virtual void IsNextNetwork(const nn::pia::local::LocalNetworkDescription*); // 0x0041E51C slot 0x10 | slot vf_0x10 of nn::pia::local::LocalMigrationManager
    virtual void GetCreateNetworkSetting(); // 0x0041E6F4 slot 0x14 | fefates:bytes
    virtual void GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription*); // 0x0041E790 slot 0x18 | fefates:bytes
    virtual void GetSubId() const; // 0x0073165C slot 0x1C | slot vf_0x1C of nn::pia::local::LocalMigrationManager
    virtual void GetLocalCommunicationId() const; // 0x0073163C slot 0x20 | fefates:bytes
    virtual void GetLocalNodeKey(unsigned short) const; // 0x007315C8 slot 0x24 | fefates:bytes
    virtual void vf_0x28(); // 0x0041E64C slot 0x28 | fefates:callseq
    UdsMigrationManagerNew(); // 0x0041E7C0 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
