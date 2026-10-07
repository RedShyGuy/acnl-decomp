#include "nn/pia/session/session_AutoMatchmakeJob.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
// 0x00436CA8
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::CreateMesh()
{
    nn::Result result = Mesh::s_pInstance->CreateMeshAsync();
    if (result.IsFailure()) {
        Session::s_pInstance->m_pMeshLayerController->vf_0x14();
        return vf_0x34(result);
    }
    Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    SetStep(&AutoMatchmakeJob::WaitCreateMesh, "AutoMatchmakeJob::WaitCreateMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00436D60
void nn::pia::session::AutoMatchmakeJob::vf_0x2C()
{
    // empty (in the original too)
}

// 0x00436D64
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::MeshStartup()
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
    m_Unknown0x58 = false;
    m_IsConnectionFailed = false;
    // the session found last: the other one of a joint session if there is one
    u32 index = pSession->m_CurrentIndex;
    if (pSession->m_SessionIds[index == 0 ? 1 : 0] != 0) {
        index = index == 0 ? 1 : 0;
    }
    nn::Result result = pSession->m_pMeshLayerController->StartupMesh(pSession->m_pMatchmakeSessions[index], false);
    if (result.IsFailure()) {
        if (m_IsCreator) {
            return vf_0x34(result);
        }
        return vf_0x38(result);
    }
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = true;
    }
    if (m_IsCreator) {
        if (Session::s_pInstance->IsUsingStationIdTable()) {
            Session* pSession2 = Session::s_pInstance;
            transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(pSession2->m_StationIdEntryNumMax[pSession2->m_CurrentIndex]);
        }
        SetStep(&AutoMatchmakeJob::CreateMesh, "AutoMatchmakeJob::CreateMesh");
    } else {
        SetStep(&AutoMatchmakeJob::GetStationConnection, "AutoMatchmakeJob::GetStationConnection");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00436F30
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::WaitJoinMesh()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (!Mesh::s_pInstance->IsJoinMeshAsyncCompleted()) {
        if (m_IsHostLeft || m_Unknown0x58 || IsCancelRequested()) {
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
            if (m_Unknown0x58) {
                result = common::RESULT_JOIN_FAILED;
            }
        }
        return vf_0x38(result);
    }
    if (Mesh::s_pInstance->CheckJoined().IsFailure()) {
        m_Result = common::RESULT_JOIN_FAILED;
        return vf_0x38(common::RESULT_NOT_JOINED);
    }
    SetStep(&AutoMatchmakeJob::CompleteProcess, "AutoMatchmakeJob::CompleteProcess");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0043709C
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::WaitLeaveMesh()
{
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    return vf_0x3C();
}

// 0x004370E0
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::WaitCreateMesh()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (!Mesh::s_pInstance->IsCreateMeshAsyncCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    Session* pSession = Session::s_pInstance;
    nn::Result result = Mesh::s_pInstance->GetCreateMeshAsyncResult();
    if (result.IsFailure()) {
        pSession->m_pMeshLayerController->vf_0x14();
        return vf_0x34(result);
    }
    pSession->m_State = 2;
    pSession->m_DisconnectState = 1;
    pSession->SetupStationIdsAsHost();
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = false;
        pSession->m_pStationIdStatusTable->NotifyStations();
    }
    vf_0x30();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004371EC
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::CompleteFailure()
{
    Session::s_pInstance->m_pMeshLayerController->vf_0x14();
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
    content.m_Unknown0x25F = Session::s_pInstance->GetAutoMatchmakePhase();
    content.m_JoinPhase = Session::s_pInstance->GetJoinMeshJobPhase();
    content.m_JoinResult = m_Result.GetPrintableBits();
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(false);
    common::g_SessionBeginMonitoringContent.Cleanup();
    common::g_SessionStateMonitoringContent.Cleanup();
    Cleanup();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00437348
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::CompleteProcess()
{
    if (IsCancelRequested()) {
        m_Result = common::RESULT_CANCELED;
        SetStep(&AutoMatchmakeJob::LeaveMesh, "AutoMatchmakeJob::LeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (m_IsHostLeft || m_Unknown0x58) {
        m_Result = common::RESULT_JOIN_FAILED;
        SetStep(&AutoMatchmakeJob::LeaveMesh, "AutoMatchmakeJob::LeaveMesh");
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
        SetStep(&AutoMatchmakeJob::LeaveMesh, "AutoMatchmakeJob::LeaveMesh");
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = false;
        pSession->m_pStationIdStatusTable->RemoveLostStations();
        pSession->m_pStationIdStatusTable->NotifyStations();
    }
    s32 msec = (common::Scheduler::s_pInstance->m_DispatchTime - m_StartTime).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
    if (msec >= 0) {
        content.m_Unknown0x180 = msec;
    }
    content.m_JoinPhase = 0xFF;
    content.m_JoinResult = m_IsCreator ? 0xFFFFFFFF : 0;
    content.m_HostPrincipalId = 0xFFFFFFFF;
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(false);
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004375D4 (name is ours)
void nn::pia::session::AutoMatchmakeJob::SetSessionState(u8 state)
{
    Session::s_pInstance->m_State = state;
}

// 0x004375E8 (name is ours)
void nn::pia::session::AutoMatchmakeJob::SetSessionDisconnectState(u8 state)
{
    Session::s_pInstance->m_DisconnectState = state;
}

// 0x004375FC
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::GetStationConnection()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    // the session found last: the other one of a joint session if there is one
    Session* pSession = Session::s_pInstance;
    CommonMatchmakeSession* pMatchmakeSession;
    u32 sessionId;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == 0) {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    } else {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
        sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    }
    transport::StationConnectionInfo info;
    m_ConnectionInfo.SetStationConnectionInfo(info);
    nn::Result result = pMatchmakeSession->vf_0x50(&m_CallContext, sessionId);
    if (result.IsFailure()) {
        return vf_0x38(result);
    }
    SetStep(&AutoMatchmakeJob::WaitGetStationConnection, "AutoMatchmakeJob::WaitGetStationConnection");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00437764
void nn::pia::session::AutoMatchmakeJob::vf_0x24(u32)
{
    // empty (in the original too)
}

// 0x00437768
void nn::pia::session::AutoMatchmakeJob::vf_0x20(u32, u32)
{
    // empty (in the original too)
}

// 0x0043776C
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::WaitGetStationConnection()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    u32 index = pSession->m_CurrentIndex;
    if (pSession->m_SessionIds[index == 0 ? 1 : 0] != 0) {
        index = index == 0 ? 1 : 0;
    }
    if (pSession->m_pMatchmakeSessions[index]->vf_0x54(&m_ConnectionInfo)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            return vf_0x38(m_CallContext.m_Result);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            m_HostPrincipalId = m_ConnectionInfo.m_PublicLocation.m_PrincipalId;
            SetStep(&AutoMatchmakeJob::JoinMesh, "AutoMatchmakeJob::JoinMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00437894 (name is ours)
bool nn::pia::session::AutoMatchmakeJob::IsCancelRequested() const
{
    return m_pCallContext != nullptr && m_pCallContext->m_IsCancelRequested;
}

// 0x004378B0
void nn::pia::session::AutoMatchmakeJob::Cleanup()
{
    vf_0x2C();
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
    m_Phase = 0;
    m_SessionId = 0;
    m_IsCreator = false;
    m_IsJoined = false;
    m_Result = nn::Result();
    m_IsMeshEvent19 = false;
    m_IsHostLeft = false;
    m_Unknown0x58 = false;
    m_IsMeshEvent20 = false;
    m_Unknown0x54 = 0;
    m_IsConnectionFailed = false;
    m_HostPrincipalId = 0;
    transport::StationConnectionInfo info;
    m_ConnectionInfo.SetStationConnectionInfo(info);
}

// 0x00437960 (name is ours)
nn::Result nn::pia::session::AutoMatchmakeJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::session::CreateSessionSetting* pCreateSetting,
                                                       const nn::pia::session::SessionSearchCriteria* pCriteria, u32 criteriaNum)
{
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pCreateSetting) || !common::IsValidPointer(pCriteria) || criteriaNum == 0 ||
        m_pCallContext != nullptr) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    nn::Result result = vf_0x28(pCreateSetting, pCriteria, criteriaNum);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_Unknown0x54 = 0;
    m_RetryCount = 0;
    Reset(true);
    common::SessionBeginMonitoringContent& content = common::g_SessionBeginMonitoringContent;
    content.m_Unknown0x22B = 0xFF;
    content.m_Unknown0x24F = 0xFF;
    content.m_Unknown0x250 = 0xFF;
    content.m_Unknown0x251 = 0xFF;
    content.m_Unknown0x252 = 0xFF;
    content.m_Unknown0x253 = 0xFF;
    content.m_Unknown0x270 = 0xFFFF;
    content.m_Unknown0x272 = 0xFFFF;
    m_StartTime = common::Scheduler::s_pInstance->m_DispatchTime;
    return nn::Result();
}

// 0x00437A84
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::JoinMesh()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = Mesh::s_pInstance->JoinMeshAsync(m_ConnectionInfo);
    if (result.IsFailure()) {
        common::g_SessionBeginMonitoringContent.m_JoinPhase = Session::GetJoinMeshJobPhase();
        return vf_0x38(result);
    }
    Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    SetStep(&AutoMatchmakeJob::WaitJoinMesh, "AutoMatchmakeJob::WaitJoinMesh");
    m_Phase = 2;
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00437B78
nn::pia::common::ExecuteResult nn::pia::session::AutoMatchmakeJob::LeaveMesh()
{
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex) {
        // the host ends the mesh or hands it over
        if (pMesh->m_IsHostMigrationEnabled) {
            Mesh::s_pInstance->LeaveMeshWithHostMigration(&m_CallContext);
        } else {
            Mesh::s_pInstance->DestroyMesh(&m_CallContext);
        }
    } else {
        Mesh::s_pInstance->LeaveMesh(&m_CallContext);
    }
    SetStep(&AutoMatchmakeJob::WaitLeaveMesh, "AutoMatchmakeJob::WaitLeaveMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00437C24
nn::pia::session::AutoMatchmakeJob::AutoMatchmakeJob()
    : m_pCallContext(nullptr), m_Phase(0), m_SessionId(0), m_IsCreator(false), m_IsJoined(false), m_Result(common::RESULT_NOT_SET)
{
}

// 0x00437CCC
// 0x00437C9C (deleting dtor)
nn::pia::session::AutoMatchmakeJob::~AutoMatchmakeJob()
{
    // empty (in the original too)
}

// 0x007338E8
u8 nn::pia::session::AutoMatchmakeJob::GetPhase() const
{
    return m_Phase;
}

// 0x007338F0
void nn::pia::session::AutoMatchmakeJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
