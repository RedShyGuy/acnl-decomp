#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalNetworkManagerE @ 0x008CFB8C
// vtable 0x00900C10 (vptr 0x00900C18), offset_to_top 0, 37 entries
class LocalNetworkManager : public ::nn::pia::common::RootObject
{
public:
    LocalNetworkManager(); // ctor candidate(s) 0x00419C4C (unverified)
    virtual void vf_0x00(); // 0x00419E88 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x04(); // 0x00419DEC slot 0x04 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x08(); // 0x004183B4 slot 0x08 | fefates:callseq-callee
    virtual void vf_0x0C(); // 0x00419AA0 slot 0x0C | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void SetConnectionStatus(const nn::pia::local::LocalConnectionStatus*); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void GetConnectionStatus(nn::pia::local::LocalConnectionStatus*) const; // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void IsDisconnectedByRequestFromSystem() const; // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void CreateLocalCommunicationId(unsigned int, bool) const; // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void GetApplicationData(void*, unsigned int*, unsigned int, const nn::pia::local::LocalNetworkDescription*) const; // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void GetSystemData(const nn::pia::local::LocalNetworkDescription*) const; // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void MakeBeaconForCreateNetwork(nn::pia::local::LocalCreateNetworkSetting*); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void GetBeaconSystemDataSize() const; // 0x0011C12F slot 0x50 | slot vf_0x00 of ChangeRentalBase
    virtual void GetBeaconApplicationDataSizeMax() const; // 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x58(); // 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x5C(); // 0x0011C12F slot 0x5C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x60(); // 0x0011C12F slot 0x60 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x64(); // 0x0011C12F slot 0x64 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x68(); // 0x0011C12F slot 0x68 | slot vf_0x00 of ChangeRentalBase
    virtual void DisallowParticipating(bool); // 0x0011C12F slot 0x6C | slot vf_0x00 of ChangeRentalBase
    virtual void AllowParticipating(); // 0x0011C12F slot 0x70 | slot vf_0x00 of ChangeRentalBase
    virtual void EjectClient(const nn::pia::common::StationAddress&); // 0x0011C12F slot 0x74 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x78(); // 0x0011C12F slot 0x78 | slot vf_0x00 of ChangeRentalBase
    virtual void CreateSessionId(); // 0x0011C12F slot 0x7C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x80(); // 0x00731184 slot 0x80 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void SetupParams(); // 0x0011C12F slot 0x84 | slot vf_0x00 of ChangeRentalBase
    virtual void StartupImpl(); // 0x0011C12F slot 0x88 | slot vf_0x00 of ChangeRentalBase
    virtual void CleanupImpl(); // 0x0011C12F slot 0x8C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x90(); // 0x0011C12F slot 0x90 | slot vf_0x00 of ChangeRentalBase
    void ClearNodeList(unsigned short); // 0x00418808 | fefates:bytes [tier B]
    void ClearNodeList(); // 0x00418844 | fefates:bytes [tier B]
    void ClearIds(); // 0x00419A4C | fefates:bytes [tier B]
    void GetNodeNum() const; // 0x00730EAC | fefates:bytes [tier B]
    void GetApplicationVersion() const; // 0x00730EDC | fefates:bytes [tier B]
    void GetConnectedTransportIdBitmap(bool) const; // 0x00730FB4 | fefates:bytes [tier B]
    void ConvertLocalNodeIdToTransportId(unsigned short) const; // 0x00731024 | fefates:bytes [tier B]
    void ConvertTransportIdToLocalNodeId(unsigned char) const; // 0x007310A0 | fefates:bytes [tier B]
    void IsHost() const; // 0x00731188 | fefates:bytes [tier B]
    void IsClient() const; // 0x007311AC | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
