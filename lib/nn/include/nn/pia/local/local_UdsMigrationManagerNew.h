#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include <cstring>

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local22UdsMigrationManagerNewE @ 0x008CFC4C
// vtable 0x00900FC0 (vptr 0x00900FC8), offset_to_top 0, 11 entries
//
// The host migration with uds: the beacon of the network carries the system data below (28
// bytes), the next host creates a network like the one of the old host and the clients find it by
// the BSSID of the old network.
class UdsMigrationManagerNew : public ::nn::pia::local::LocalMigrationManager
{
public:
    // the system data in the beacon (the member names are ours)
    struct BeaconSystemData : public ::nn::pia::local::LocalBeaconSystemData
    {
        BeaconSystemData()
        {
            std::memset(m_Bssid, 0, sizeof(m_Bssid));
            std::memset(m_Reserved2, 0, sizeof(m_Reserved2));
        }

        u8 m_Bssid[6];     // 0x10, of the network (the next network keeps it)
        u8 m_Reserved2[6]; // 0x16
    };

    static const u32 SYSTEM_DATA_SIZE = sizeof(BeaconSystemData);
    static const u32 APPLICATION_DATA_SIZE_MAX = 200 - SYSTEM_DATA_SIZE;

    UdsMigrationManagerNew(); // 0x0041E7C0 | fefates:bytes [tier B]
    virtual ~UdsMigrationManagerNew(); // 0x0041E908 slot 0x00 | fefates:bytes
    // 0x0041E89C slot 0x04 (deleting dtor)
    virtual void SetupParams(); // 0x0041E4F4 slot 0x08 | fefates:bytes
    virtual nn::Result SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting& setting); // 0x0041E5D0 slot 0x0C | fefates:bytes
    virtual bool IsNextNetwork(const nn::pia::local::LocalNetworkDescription* pDescription); // 0x0041E51C slot 0x10
    virtual nn::pia::local::LocalCreateNetworkSetting* GetCreateNetworkSetting(); // 0x0041E6F4 slot 0x14 | fefates:bytes
    virtual nn::pia::local::LocalConnectNetworkSetting* GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription* pDescription); // 0x0041E790 slot 0x18 | fefates:bytes
    virtual u8 GetSubId() const; // 0x0073165C slot 0x1C
    virtual u32 GetLocalCommunicationId() const; // 0x0073163C slot 0x20 | fefates:bytes
    virtual u64 GetLocalNodeKey(u16 nodeId) const; // 0x007315C8 slot 0x24 | fefates:bytes
    virtual void SetSystemDataToBeacon(void* pBeacon); // 0x0041E64C slot 0x28 | fefates:callseq
};
ASSERT_SIZE(UdsMigrationManagerNew::BeaconSystemData, 28);
ASSERT_SIZE(UdsMigrationManagerNew, 0x4C);
} // namespace local
} // namespace pia
} // namespace nn
