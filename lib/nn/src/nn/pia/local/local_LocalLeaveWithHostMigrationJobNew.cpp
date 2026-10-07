#include "nn/pia/local/local_LocalLeaveWithHostMigrationJobNew.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/local/local_LocalMigrationManager.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace local {
// 0x0042554C
nn::pia::StationIndex nn::pia::local::LocalLeaveWithHostMigrationJobNew::DecideNextHost()
{
    transport::Station* pLocalStation = transport::StationManager::s_pInstance->m_pLocalStation;
    u8 next = LocalNetwork::s_pInstance->m_pMigrationManager->GetNextHostCandidateTransportId();
    LocalNetworkManager* pManager = LocalNetwork::s_pInstance->m_pNetworkManager;
    u8 localTransportId = pManager->m_LocalTransportId;
    if (pLocalStation == nullptr || pManager->m_InvalidNodeId == localTransportId ||
        pLocalStation->m_StationAddress.GetExtensionId() == pManager->m_InvalidNodeId ||
        pLocalStation->m_StationAddress.GetExtensionId() != localTransportId || pManager->m_InvalidNodeId == next ||
        next == localTransportId) {
        return STATION_INDEX_UNIDENTIFIED;
    }

    common::StationAddress address;
    address.SetExtensionId(next);
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(address);
    if (pStation == nullptr) {
        return STATION_INDEX_UNIDENTIFIED;
    }
    return pStation->m_StationIndex;
}

// 0x00425628
nn::pia::local::LocalLeaveWithHostMigrationJobNew::LocalLeaveWithHostMigrationJobNew()
{
    // only the base and the vptr (in the original too)
}

// 0x00425650
// 0x00425640 (deleting dtor)
nn::pia::local::LocalLeaveWithHostMigrationJobNew::~LocalLeaveWithHostMigrationJobNew()
{
    // empty (in the original too)
}

// 0x007317F8
void nn::pia::local::LocalLeaveWithHostMigrationJobNew::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
