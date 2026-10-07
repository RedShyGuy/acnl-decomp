#include "nn/pia/session/session_MeshEventListener.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_DisconnectStationJob.h"
#include "nn/pia/transport/transport_IdentificationInfoTable.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
// 0x00438F54 slot 0x00 (name is ours)
void nn::pia::session::MeshEventListener::OnEvent(const nn::pia::session::Mesh::Event&)
{
    // empty (in the original too)
}

// 0x00438F58 slot 0x04 (name is ours)
void nn::pia::session::MeshEventListener::OnLeave(nn::pia::StationIndex stationIndex)
{
    Mesh::s_pInstance->UnfixDisconnectedId(stationIndex);
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_StationNum != 0) {
        pMesh->m_StationNum--;
    }
    transport::Station* pStation = transport::StationManager::s_pInstance->GetStation(stationIndex);
    if (pStation == nullptr) {
        return;
    }
    pStation->m_pDisconnectStationJob->OnDisconnected(pStation);
    transport::StationConnectionInfoTable::s_pInstance->EraseFromTable(pStation);
    transport::IdentificationInfoTable::s_pInstance->EraseFromTable(pStation);
    pStation->CleanupJobs();
    pStation->Cleanup();
    transport::StationManager::s_pInstance->DestroyStation(pStation);
    transport::Transport::s_pInstance->OutputStreamUpdateEvent();
}

// 0x00439010
nn::pia::session::MeshEventListener::MeshEventListener()
{
    // only the vptr (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
