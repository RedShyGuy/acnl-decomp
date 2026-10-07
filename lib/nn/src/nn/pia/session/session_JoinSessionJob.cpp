#include "nn/pia/session/session_JoinSessionJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
// 0x004315A0
void nn::pia::session::JoinSessionJob::vf_0x34()
{
    // empty (in the original too)
}

// 0x004315A4
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::MeshCleanup()
{
    Session::s_pInstance->m_pMeshLayerController->vf_0x14();
    return vf_0x40();
}

// 0x004315DC
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::MeshStartup()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    m_IsMeshEvent19 = false;
    m_IsMeshEvent20 = false;
    m_IsHostLeft = false;
    m_Unknown0xB9 = false;
    m_IsConnectionFailed = false;
    // the session joined last: the other one of a joint session if there is one
    u32 index = pSession->m_CurrentIndex;
    if (pSession->m_SessionIds[index == 0 ? 1 : 0] != 0) {
        index = index == 0 ? 1 : 0;
    }
    nn::Result result = pSession->m_pMeshLayerController->StartupMesh(pSession->m_pMatchmakeSessions[index], false);
    if (result.IsFailure()) {
        return vf_0x38(result);
    }
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = true;
    }
    SetStep(&JoinSessionJob::JoinMesh, "JoinSessionJob::JoinMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00431704
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::WaitJoinMesh()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (!Mesh::s_pInstance->IsJoinMeshAsyncCompleted()) {
        if (m_IsHostLeft || m_Unknown0xB9 || IsCancelRequested()) {
            Mesh::s_pInstance->CancelJoinMeshAsync();
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    nn::Result result = Mesh::s_pInstance->GetJoinMeshAsyncResult();
    if (result.IsFailure()) {
        // a join that was canceled because the host left failed
        if (result == common::RESULT_CANCELED) {
            if (m_IsHostLeft) {
                result = common::RESULT_JOIN_FAILED;
            }
            if (m_Unknown0xB9) {
                result = common::RESULT_JOIN_FAILED;
            }
        }
        return vf_0x38(result);
    }
    if (Mesh::s_pInstance->CheckJoined().IsFailure()) {
        m_Result = common::RESULT_JOIN_FAILED;
        return vf_0x38(common::RESULT_NOT_JOINED);
    }
    SetStep(&JoinSessionJob::CompleteProcess, "JoinSessionJob::CompleteProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043186C
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::WaitLeaveMesh()
{
    if (!Mesh::s_pInstance->IsLeaveMeshAsyncCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    Mesh::s_pInstance->GetLeaveMeshAsyncResult();
    return vf_0x3C();
}

// 0x004318CC
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::CompleteFailure()
{
    if (IsCancelRequested() || m_Result == common::RESULT_CANCELED) {
        m_pCallContext->SignalCancel();
        m_Result = common::RESULT_CANCELED;
    } else if (m_Result == common::RESULT_INVALID_STATE || m_Result == common::RESULT_NOT_IN_SESSION) {
        Session::s_pInstance->SetDisconnected();
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_Result = common::RESULT_NOT_IN_SESSION;
    } else {
        m_pCallContext->SignalFailure(m_Result);
    }
    m_pCallContext = nullptr;
    s32 msec = (common::Scheduler::s_pInstance->m_DispatchTime - m_StartTime).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    if (msec >= 0) {
        content.m_Unknown0x180 = msec;
    }
    content.m_Unknown0x25E = Session::s_pInstance->GetJoinSessionPhase();
    content.m_JoinPhase = Session::s_pInstance->GetJoinMeshJobPhase();
    content.m_JoinResult = m_Result.GetPrintableBits();
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(false);
    common::g_SessionBeginMonitoringContent.Cleanup();
    common::g_SessionStateMonitoringContent.Cleanup();
    Cleanup();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00431A24
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::CompleteProcess()
{
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&JoinSessionJob::LeaveMesh, "JoinSessionJob::LeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_IsHostLeft || m_Unknown0xB9) {
        m_Result = common::RESULT_JOIN_FAILED;
        SetStep(&JoinSessionJob::LeaveMesh, "JoinSessionJob::LeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    m_IsJoined = true;
    Session* pSession = Session::s_pInstance;
    // a joint session if the other session is joined too
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0) {
        pSession->m_State = 4;
        if (pSession->IsUsingStationIdTable()) {
            Session* pSession2 = Session::s_pInstance;
            transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(
                pSession2->m_StationIdEntryNumMax[pSession2->m_CurrentIndex == 0 ? 1 : 0]);
        }
    } else {
        pSession->m_State = 2;
        if (pSession->IsUsingStationIdTable()) {
            Session* pSession2 = Session::s_pInstance;
            transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(pSession2->m_StationIdEntryNumMax[pSession2->m_CurrentIndex]);
        }
    }
    pSession->m_DisconnectState = 1;
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    if (!pSession->SetupStationIdsAsClient()) {
        m_Result = common::RESULT_JOIN_FAILED;
        content.m_JoinPhase = Session::GetJoinMeshJobPhase();
        SetStep(&JoinSessionJob::LeaveMesh, "JoinSessionJob::LeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = false;
        pSession->m_pStationIdStatusTable->RemoveLostStations();
        pSession->m_pStationIdStatusTable->NotifyStations();
    }
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    s32 msec = (common::Scheduler::s_pInstance->m_DispatchTime - m_StartTime).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
    if (msec >= 0) {
        content.m_Unknown0x180 = msec;
    }
    content.m_JoinPhase = 0xFF;
    content.m_JoinResult = 0;
    content.m_HostPrincipalId = 0xFFFFFFFF;
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(false);
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00431CAC (name is ours)
void nn::pia::session::JoinSessionJob::SetSessionState(u8 state)
{
    Session::s_pInstance->m_State = state;
}

// 0x00431CC0 (name is ours)
void nn::pia::session::JoinSessionJob::SetSessionDisconnectState(u8 state)
{
    Session::s_pInstance->m_DisconnectState = state;
}

// 0x00431CD4
void nn::pia::session::JoinSessionJob::vf_0x1C(u32)
{
    // empty (in the original too)
}

// 0x00431CD8
void nn::pia::session::JoinSessionJob::vf_0x18(u32, u32)
{
    // empty (in the original too)
}

// 0x00431CDC (name is ours)
void nn::pia::session::JoinSessionJob::CancelCall()
{
    m_CallContext.SignalCancel();
    m_CallContext.Reset();
}

// 0x00431CF8 (name is ours)
bool nn::pia::session::JoinSessionJob::IsCancelRequested()
{
    return m_pCallContext != nullptr && m_pCallContext->m_IsCancelRequested;
}

// 0x00431D14 (name is ours)
void nn::pia::session::JoinSessionJob::Cleanup()
{
    vf_0x34();
    if (m_pCallContext != nullptr) {
        if (m_pCallContext->m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
            m_pCallContext->SignalCancel();
        }
        m_pCallContext = nullptr;
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    m_IsJoined = false;
    m_IsHostLeft = false;
    m_Unknown0xB9 = false;
    m_IsMeshEvent19 = false;
    m_IsMeshEvent20 = false;
    m_Unknown0xB5 = false;
    m_IsConnectionFailed = false;
    m_Phase = 0;
    m_Result = nn::Result();
    transport::StationConnectionInfo info;
    m_ConnectionInfo.SetStationConnectionInfo(info);
}

// 0x0073387C (name is ours)
void nn::pia::session::JoinSessionJob::SetJoinTimeToMonitoringContent()
{
    s32 msec = (common::Scheduler::s_pInstance->m_DispatchTime - m_StartTime).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
    if (msec >= 0) {
        common::g_SessionBeginMonitoringContent.m_Unknown0x180 = msec;
    }
}

// 0x00431DB8 (name is ours)
nn::Result nn::pia::session::JoinSessionJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::session::JoinSessionSetting* pSetting)
{
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSetting)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    nn::Result result = vf_0x30(pSetting);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_Unknown0xB5 = false;
    m_RetryCount = 0;
    Reset(true);
    m_StartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x22B = 0xFF;
    content.m_Unknown0x24F = 0xFF;
    content.m_Unknown0x250 = 0xFF;
    content.m_Unknown0x251 = 0xFF;
    content.m_Unknown0x252 = 0xFF;
    content.m_Unknown0x253 = 0xFF;
    content.m_Unknown0x270 = 0xFFFF;
    content.m_Unknown0x272 = 0xFFFF;
    return nn::Result();
}

// 0x00431EB0
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::JoinMesh()
{
    if (IsCancelRequested()) {
        return vf_0x38(common::RESULT_CANCELED);
    }
    nn::Result result = Mesh::s_pInstance->JoinMeshAsync(m_ConnectionInfo);
    if (result.IsFailure()) {
        common::g_SessionBeginMonitoringContent.m_JoinPhase = Session::GetJoinMeshJobPhase();
        Session::s_pInstance->m_pMeshLayerController->vf_0x14();
        return vf_0x38(result);
    }
    Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    SetStep(&JoinSessionJob::WaitJoinMesh, "JoinSessionJob::WaitJoinMesh");
    m_Phase = 2;
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00431FC8
nn::pia::common::ExecuteResult nn::pia::session::JoinSessionJob::LeaveMesh()
{
    Mesh::s_pInstance->LeaveMeshAsync();
    SetStep(&JoinSessionJob::WaitLeaveMesh, "JoinSessionJob::WaitLeaveMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00432030
nn::pia::session::JoinSessionJob::JoinSessionJob() : m_pCallContext(nullptr), m_Phase(0), m_IsJoined(false), m_Result(common::RESULT_NOT_SET)
{
}

// 0x004320CC
// 0x0043209C (deleting dtor)
nn::pia::session::JoinSessionJob::~JoinSessionJob()
{
    // empty (in the original too)
}

// 0x007338C8
void nn::pia::session::JoinSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00733874
u8 nn::pia::session::JoinSessionJob::GetPhase() const
{
    return m_Phase;
}

} // namespace session
} // namespace pia
} // namespace nn
