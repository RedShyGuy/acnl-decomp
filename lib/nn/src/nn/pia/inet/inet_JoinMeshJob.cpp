#include "nn/pia/inet/inet_JoinMeshJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/inet/inet_NexFacade.h"
#include "nn/pia/session/session_KickoutManageJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_ProcessHostMigrationJob.h"
#include "nn/pia/transport/transport_BandwidthCheckerProtocol.h"
#include "nn/pia/transport/transport_Station.h"
#include "nn/pia/transport/transport_StationConnectionInfoTable.h"
#include "nn/pia/transport/transport_StationManager.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TABLE_TRACE_FLAG = 0x80000000ULL;
} // namespace

// (inline)
inline void nn::pia::inet::JoinMeshJob::SetLeaveStep()
{
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (session::Mesh::s_pInstance->m_IsHostMigrationEnabled && pMesh->m_LocalStationIndex <= STATION_INDEX_MAX &&
        pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        SetStep(&JoinMeshJob::LeaveMeshWithHostMigration, "JoinMeshJob::LeaveMeshWithHostMigration");
    } else {
        SetStep(&JoinMeshJob::LeaveMesh, "JoinMeshJob::LeaveMesh");
    }
}

// 0x003E26BC | slot vf_0x1C of nn::pia::session::JoinMeshJob
void nn::pia::inet::JoinMeshJob::CleanupImpl()
{
    // empty (in the original too)
}

// 0x003E26C0 | slot vf_0x18 of nn::pia::session::JoinMeshJob
bool nn::pia::inet::JoinMeshJob::StartupImpl()
{
    Reset(true);
    // (both branches of the original, with and without the NAT session, set this step)
    SetStep(&JoinMeshJob::StartConnectingToHost, "JoinMeshJob::StartConnectingToHost");
    common::g_SessionBeginMonitoringContent.m_Unknown0x58 = NexFacade::s_pInstance->m_Unknown0x10;
    return true;
}

// 0x003E276C
nn::pia::common::ExecuteResult nn::pia::inet::JoinMeshJob::WaitBandWidthCheck()
{
    transport::BandwidthCheckerProtocol* pChecker = session::Mesh::s_pInstance->m_pBandwidthCheckerProtocol;
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        pChecker->Cancel();
        SetLeaveStep();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (CheckTransportConnectionStatus() || (!session::Mesh::s_pInstance->m_IsHostMigrationEnabled && CheckConnectionStateWithHostStation())) {
        pChecker->Cancel();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (!pChecker->IsCheckDone()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (session::Mesh::s_pInstance->m_IsHostMigrationEnabled) {
        SetStep(&JoinMeshJob::WaitCheckHostConnection, "JoinMeshJob::WaitCheckHostConnection");
    } else {
        SetStep(&JoinMeshJob::WaitCheckKickoutJob, "JoinMeshJob::WaitCheckKickoutJob");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x003E2960
nn::pia::common::ExecuteResult nn::pia::inet::JoinMeshJob::StartBandWidthCheck()
{
    transport::BandwidthCheckerProtocol* pChecker = session::Mesh::s_pInstance->m_pBandwidthCheckerProtocol;
    if (pChecker->m_IsOneWay) {
        SetStep(&JoinMeshJob::CompleteProcess, "JoinMeshJob::CompleteProcess");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_Phase = PHASE_BANDWIDTH_CHECK;
    if (m_StationNum >= 2) {
        if (!pChecker->IsCheckStartable()) {
            pChecker->Cancel();
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        transport::StationConnectionInfoTable::s_pInstance->Trace(TABLE_TRACE_FLAG);
        // the check goes to a station of the response from its end, not the own one
        transport::Station* pStation = nullptr;
        for (u32 i = 2; i <= m_StationNum; i++) {
            pStation = transport::StationConnectionInfoTable::s_pInstance->GetStation(m_pStationConnectionInfos[m_StationNum - i]);
            if (pStation != nullptr && pStation->m_StationIndex != STATION_INDEX_UNIDENTIFIED &&
                pStation->m_StationIndex != session::Mesh::s_pInstance->m_LocalStationIndex) {
                break;
            }
        }
        if (pStation == nullptr) {
            SetStep(&JoinMeshJob::WaitBandWidthCheck, "JoinMeshJob::WaitBandWidthCheck");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        pChecker->StartCheck(pStation->m_StationIndex);
    }
    SetStep(&JoinMeshJob::WaitBandWidthCheck, "JoinMeshJob::WaitBandWidthCheck");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x003E2B1C
nn::pia::common::ExecuteResult nn::pia::inet::JoinMeshJob::WaitCheckKickoutJob()
{
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        SetLeaveStep();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (CheckTransportConnectionStatus()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (session::Mesh::s_pInstance->m_pKickoutManageJob->IsRunning()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&JoinMeshJob::CompleteProcess, "JoinMeshJob::CompleteProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x003E2C88
nn::pia::common::ExecuteResult nn::pia::inet::JoinMeshJob::WaitCheckHostConnection()
{
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        SetLeaveStep();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (CheckTransportConnectionStatus()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    session::Mesh* pMesh = session::Mesh::s_pInstance;
    if (pMesh->m_pProcessHostMigrationJob->m_IsRunning || pMesh->m_pKickoutManageJob->IsRunning()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    transport::Station* pHost = transport::StationManager::s_pInstance->GetStation(session::Mesh::s_pInstance->m_HostStationIndex);
    if (pHost == nullptr || pHost->m_State != transport::Station::STATION_STATE_CONNECTED) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&JoinMeshJob::CompleteProcess, "JoinMeshJob::CompleteProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x003E2E4C | slot vf_0x20 of nn::pia::session::JoinMeshJob
void nn::pia::inet::JoinMeshJob::SetHostInfoToMonitoringData(const nn::pia::transport::StationConnectionInfo& info)
{
    common::g_SessionBeginMonitoringContent.m_Unknown0x168 = common::hashWithMd5(info.m_PublicLocation.m_PrincipalId);
}

// 0x003E2E68 | virtual slot, introduced by nn::pia::session::JoinMeshJob
nn::pia::common::ExecuteResult nn::pia::inet::JoinMeshJob::ProceedToCompleteProcess()
{
    if (session::Mesh::s_pInstance->m_IsBandwidthCheckEnabled) {
        SetStep(&JoinMeshJob::StartBandWidthCheck, "JoinMeshJob::StartBandWidthCheck");
    } else {
        SetStep(&JoinMeshJob::CompleteProcess, "JoinMeshJob::CompleteProcess");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0042C300 | slot vf_0x00 of nn::pia::common::Job
// 0x004266A4 (deleting dtor)
nn::pia::inet::JoinMeshJob::~JoinMeshJob()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
