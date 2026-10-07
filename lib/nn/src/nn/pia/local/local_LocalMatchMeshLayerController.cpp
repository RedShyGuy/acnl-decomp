#include "nn/pia/local/local_LocalMatchMeshLayerController.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalFacade.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
// 0x004232F4
void nn::pia::local::LocalMatchMeshLayerController::vf_0x14()
{
    CleanupMesh();
    LocalFacade::s_pInstance->Cleanup();
}

// 0x00423318
u8 nn::pia::local::LocalMatchMeshLayerController::GetNetworkStatus()
{
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (!m_IsJoined) {
        if (pNetwork->IsHost() || pNetwork->IsClient()) {
            m_IsJoined = true;
        }
        return 1;
    }
    if (pNetwork->IsHost() || pNetwork->IsClient()) {
        return 1;
    }
    if (!LocalNetwork::s_pInstance->IsDuringHostMigration()) {
        return 3;
    }
    return 1;
}

// 0x004233B0
void nn::pia::local::LocalMatchMeshLayerController::Cleanup()
{
    vf_0x14();
    LocalNetwork::s_pInstance->Cleanup();
    ClearUnknown0x69();
    m_IsJoined = false;
}

// 0x004233EC
nn::Result nn::pia::local::LocalMatchMeshLayerController::Startup(bool isHostMigrationEnabled, const u8* pIdentificationData, nn::pia::common::Crypto::Mode cryptoMode,
                                                                  u32 timeoutMSec, u32 keepAliveIntervalMSec, s32 bandwidthCheckBandwidth, u32 bandwidthCheckPacketSize,
                                                                  bool isBandwidthCheckOneWay, s32 bandwidthCheckDurationMSec,
                                                                  const nn::pia::transport::Station::PlayerName* pPlayerName, bool value)
{
    // (the result of the base is not used)
    MeshLayerController::Startup(isHostMigrationEnabled, pIdentificationData, cryptoMode, timeoutMSec, keepAliveIntervalMSec, bandwidthCheckBandwidth,
                                 bandwidthCheckPacketSize, isBandwidthCheckOneWay, bandwidthCheckDurationMSec, pPlayerName, value);
    nn::Result result = LocalNetwork::s_pInstance->Startup();
    if (result.IsFailure()) {
        if (result == common::RESULT_LOCAL_NETWORK_LOST) {
            return common::RESULT_NOT_IN_SESSION;
        }
        return result;
    }
    return result;
}

// 0x00423438
// 0x00423434 (deleting dtor)
nn::pia::local::LocalMatchMeshLayerController::~LocalMatchMeshLayerController()
{
    // empty (in the original too)
}

// 0x007316D0
u32 nn::pia::local::LocalMatchMeshLayerController::vf_0x28()
{
    return LocalNetwork::s_pInstance->GetConnectedNodeNum();
}

// 0x007316E0
void nn::pia::local::LocalMatchMeshLayerController::vf_0x1C()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
