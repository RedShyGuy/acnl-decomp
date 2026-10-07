#include "nn/pia/inet/inet_NexDisconnectStationJob.h"
#include "nn/pia/inet/inet_NatTraverser.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_StationLocation.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00403EF8
void nn::pia::inet::NexDisconnectStationJob::OnDisconnected(nn::pia::transport::Station* pStation)
{
    transport::StationConnectionInfoTable* pTable = transport::StationConnectionInfoTable::s_pInstance;
    transport::StationConnectionInfo info;
    if (pTable->GetStationConnectionInfo(pStation, &info).IsFailure()) {
        return;
    }
    // the station is gone for good unless the game says otherwise
    bool isGone = true;
    s32 (*pCallback)(const transport::StationIdTable::Entry*) = transport::Transport::s_pInstance->m_pStationIdCallback;
    if (pCallback != nullptr) {
        u32 principalId;
        if (pStation->GetPrincipalId(&principalId).IsFailure()) {
            return;
        }
        if (transport::Transport::s_pInstance->m_IsUsingStationIdTable) {
            transport::StationIdTable::Entry entry;
            if (transport::Transport::s_pInstance->m_pStationIdTable->Find(&entry, principalId).IsSuccess() && pCallback(&entry) == 1) {
                isGone = false;
            }
        }
    }
    transport::StationLocation location;
    location = info.m_PublicLocation;
    location.SetStationAddress(pStation->m_StationAddress);
    NexFacade::s_pInstance->m_pNatTraverser->ClearNatTraversal(location, isGone);
    // (a trace of the info; armlink removed the call)
    if (isGone && !session::Mesh::s_pInstance->IsLeaving()) {
        NexFacade::s_pInstance->m_pNatTraverser->ClearMonitoringNatTraversal(location);
    }
}

// 0x00404058
nn::pia::inet::NexDisconnectStationJob::NexDisconnectStationJob()
{
    // only the vptr (in the original too)
}

// 0x004588E4 | slot vf_0x00 of nn::pia::common::Job
// 0x00404070 (deleting dtor)
nn::pia::inet::NexDisconnectStationJob::~NexDisconnectStationJob()
{
    // empty (in the original too)
}

// 0x0072F518 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
void nn::pia::inet::NexDisconnectStationJob::Trace(unsigned long long) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
