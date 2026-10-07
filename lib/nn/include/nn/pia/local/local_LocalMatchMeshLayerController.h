#pragma once

#include "decomp.h"
#include "nn/pia/session/session_MeshLayerController.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local29LocalMatchMeshLayerControllerE @ 0x008CFD3C
// vtable 0x009012AC (vptr 0x009012B4), offset_to_top 0, 15 entries
//
// The mesh layer of the local network: the network starts and ends with the mesh, LocalFacade
// ends with it. The member name is ours.
class LocalMatchMeshLayerController : public ::nn::pia::session::MeshLayerController
{
public:
    // (inline in LocalNetworkFactory)
    LocalMatchMeshLayerController() : m_IsJoined(false) {}
    virtual ~LocalMatchMeshLayerController(); // 0x00423438 slot 0x00
    // 0x00423434 slot 0x04 (deleting dtor)
    virtual nn::Result Startup(bool isHostMigrationEnabled, const u8* pIdentificationData, nn::pia::common::Crypto::Mode cryptoMode,
                               u32 timeoutMSec, u32 keepAliveIntervalMSec, s32 bandwidthCheckBandwidth, u32 bandwidthCheckPacketSize,
                               bool isBandwidthCheckOneWay, s32 bandwidthCheckDurationMSec,
                               const nn::pia::transport::Station::PlayerName* pPlayerName, bool value); // 0x004233EC slot 0x08
    virtual void Cleanup(); // 0x004233B0 slot 0x0C
    // the mesh and LocalFacade end
    virtual void vf_0x14(); // 0x004232F4 slot 0x14
    // 3 if the network is lost (and no host migration runs), else 1
    virtual u8 GetNetworkStatus(); // 0x00423318 slot 0x18
    // (empty)
    virtual void vf_0x1C(); // 0x007316E0 slot 0x1C
    // the number of the connected nodes of the network
    virtual u32 vf_0x28(); // 0x007316D0 slot 0x28

    bool m_IsJoined; // 0x6A, the network was joined (in the tail padding of the base)
};
ASSERT_OFFSET(LocalMatchMeshLayerController, m_IsJoined, 0x6A);
} // namespace local
} // namespace pia
} // namespace nn
