#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local17UdsNetworkManagerE @ 0x008CFB68
// vtable 0x00900AA8 (vptr 0x00900AB0), offset_to_top 0, 39 entries
class UdsNetworkManager : public ::nn::pia::local::LocalNetworkManager
{
public:
    UdsNetworkManager(); // ctor candidate(s) 0x00417FDC (unverified)
    virtual void vf_0x00(); // 0x00418090 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x04(); // 0x00418058 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x08(); // 0x00416DC8 slot 0x08 | fefates:callseq
    virtual void vf_0x0C(); // 0x00417F20 slot 0x0C | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x10(); // 0x00417C40 slot 0x10 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x14(); // 0x0041709C slot 0x14 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x18(); // 0x004174D4 slot 0x18 | fefates:callseq
    virtual void vf_0x1C(); // 0x00416F2C slot 0x1C | fefates:callseq
    virtual void vf_0x20(); // 0x0041718C slot 0x20 | fefates:callseq
    virtual void SetConnectionStatus(const nn::pia::local::LocalConnectionStatus*); // 0x00417B08 slot 0x24 | slot vf_0x24 of nn::pia::local::LocalNetworkManager
    virtual void GetConnectionStatus(nn::pia::local::LocalConnectionStatus*) const; // 0x007307A8 slot 0x28 | slot vf_0x28 of nn::pia::local::LocalNetworkManager
    virtual void IsDisconnectedByRequestFromSystem() const; // 0x00730E14 slot 0x2C | slot vf_0x2C of nn::pia::local::LocalNetworkManager
    virtual void CreateLocalCommunicationId(unsigned int, bool) const; // 0x00468B04 slot 0x30 | slot vf_0x30 of nn::pia::local::LocalNetworkManager
    virtual void GetApplicationData(void*, unsigned int*, unsigned int, const nn::pia::local::LocalNetworkDescription*) const; // 0x007306B4 slot 0x34 | slot vf_0x34 of nn::pia::local::LocalNetworkManager
    virtual void vf_0x38(); // 0x0073091C slot 0x38 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void GetSystemData(const nn::pia::local::LocalNetworkDescription*) const; // 0x007302E8 slot 0x3C | slot vf_0x3C of nn::pia::local::LocalNetworkManager
    virtual void vf_0x40(); // 0x00730B18 slot 0x40 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x44(); // 0x00730CE0 slot 0x44 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x48(); // 0x00417E04 slot 0x48 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void MakeBeaconForCreateNetwork(nn::pia::local::LocalCreateNetworkSetting*); // 0x00417D6C slot 0x4C | slot vf_0x4C of nn::pia::local::LocalNetworkManager
    virtual void GetBeaconSystemDataSize() const; // 0x007309DC slot 0x50 | fefates:bytes
    virtual void GetBeaconApplicationDataSizeMax() const; // 0x00730C98 slot 0x54 | fefates:bytes-fuzzy
    virtual void vf_0x58(); // 0x0073039C slot 0x58 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x5C(); // 0x0041730C slot 0x5C | fefates:callseq
    virtual void vf_0x60(); // 0x007302B8 slot 0x60 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x64(); // 0x00730590 slot 0x64 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void vf_0x68(); // 0x00730674 slot 0x68 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void DisallowParticipating(bool); // 0x00417B64 slot 0x6C | fefates:bytes
    virtual void AllowParticipating(); // 0x00417238 slot 0x70 | fefates:bytes
    virtual void EjectClient(const nn::pia::common::StationAddress&); // 0x00416FD4 slot 0x74 | fefates:bytes
    virtual void vf_0x78(); // 0x004170D8 slot 0x78 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void CreateSessionId(); // 0x00417118 slot 0x7C | slot vf_0x7C of nn::pia::local::LocalNetworkManager
    virtual void vf_0x80(); // 0x00731180 slot 0x80 | virtual slot, introduced by nn::pia::local::LocalNetworkManager
    virtual void SetupParams(); // 0x0041701C slot 0x84 | slot vf_0x84 of nn::pia::local::LocalNetworkManager
    virtual void StartupImpl(); // 0x00417040 slot 0x88 | slot vf_0x88 of nn::pia::local::LocalNetworkManager
    virtual void CleanupImpl(); // 0x00469ED4 slot 0x8C | slot vf_0x8C of nn::pia::local::LocalNetworkManager
    virtual void vf_0x90(); // 0x00417BA0 slot 0x90 | fefates:callseq
    virtual void ProcessConnectEvent(const nn::uds::CTR::ConnectionStatus&, unsigned int); // 0x004178B8 slot 0x94 | slot vf_0x94 of nn::pia::local::UdsNetworkManager
    virtual void vf_0x98(); // 0x00417C48 slot 0x98 | fefates:callseq
    void ConvertUdsResult(const nn::Result&) const; // 0x00730480 | fefates:bytes [tier B]
    void ConvertUdsSendToResult(const nn::Result&) const; // 0x00730804 | fefates:bytes [tier B]
    void ConvertUdsReceiveFromResult(const nn::Result&) const; // 0x00730A0C | fefates:bytes [tier B]
    void ConvertLocalNodeIdToStationInfoRole(unsigned short) const; // 0x00730E50 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
