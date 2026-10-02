#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalMigrationManagerE @ 0x008CFC10
// vtable 0x00900EFC (vptr 0x00900F04), offset_to_top 0, 11 entries
class LocalMigrationManager : public ::nn::pia::common::RootObject
{
public:
    virtual ~LocalMigrationManager(); // 0x0041D524 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x0041D4E8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMigrationManager
    virtual void SetupParams(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void SetNetworkInfo(const nn::pia::local::LocalConnectNetworkSetting&); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void IsNextNetwork(const nn::pia::local::LocalNetworkDescription*); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void GetCreateNetworkSetting(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void GetConnectNetworkSetting(nn::pia::local::LocalNetworkDescription*); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void GetSubId() const; // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void GetLocalCommunicationId() const; // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void GetLocalNodeKey(unsigned short) const; // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    void Initialize(); // 0x0041CBE4 | fefates:bytes [tier B]
    void ClearNodeInfo(unsigned char); // 0x0041CD78 | fefates:bytes [tier B]
    void ClearNodeInfo(); // 0x0041CDF8 | fefates:bytes [tier B]
    void CancelHostMigration(); // 0x0041D198 | fefates:bytes [tier B]
    void SetTransportIdToNodeIdTable(unsigned char, unsigned short); // 0x0041D1AC | fefates:bytes [tier B]
    void ClearTransportIdToNodeIdTable(unsigned char); // 0x0041D1D0 | fefates:bytes [tier B]
    void ClearTransportIdToNodeIdTable(); // 0x0041D208 | fefates:bytes [tier B]
    void Cleanup(); // 0x0041D258 | fefates:bytes [tier B]
    void Startup(); // 0x0041D2A8 | fefates:bytes [tier B]
    void Finalize(); // 0x0041D3A0 | fefates:bytes [tier B]
    LocalMigrationManager(); // 0x0041D478 | fefates:bytes [tier B]
    void GetMigrationState(unsigned char) const; // 0x00731238 | fefates:bytes [tier B]
    void IsExistStateMigrating() const; // 0x00731264 | fefates:bytes [tier B]
    void RemoveBeaconSystemData(void*, const void*, unsigned int) const; // 0x007312A8 | fefates:bytes [tier B]
    void ConvertLocalNodeIdToTransportId(unsigned short) const; // 0x007313D4 | fefates:bytes [tier B]
    void ConvertTransportIdToLocalNodeId(unsigned char) const; // 0x00731444 | fefates:bytes [tier B]
    void GetNextHostCandidateTransportId() const; // 0x0073147C | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
