#include "nn/pia/session/session_CreateSessionJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
// 0x00437CF4
nn::pia::common::ExecuteResult nn::pia::session::CreateSessionJob::CreateMesh()
{
    nn::Result result = Mesh::s_pInstance->CreateMesh(&m_CallContext);
    if (result.IsFailure()) {
        Session::s_pInstance->m_pMeshLayerController->vf_0x14();
        return vf_0x24(result);
    }
    Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    SetStep(&CreateSessionJob::WaitCreateMesh, "CreateSessionJob::WaitCreateMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00437DB0
nn::pia::common::ExecuteResult nn::pia::session::CreateSessionJob::MeshStartup()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    nn::Result result = pSession->m_pMeshLayerController->StartupMesh(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex], false);
    if (result.IsFailure()) {
        return vf_0x24(result);
    }
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = true;
    }
    if (Session::s_pInstance->IsUsingStationIdTable()) {
        Session* pSession2 = Session::s_pInstance;
        transport::Transport::s_pInstance->m_pStationIdTable->SetEntryNumMax(pSession2->m_StationIdEntryNumMax[pSession2->m_CurrentIndex]);
    }
    SetStep(&CreateSessionJob::CreateMesh, "CreateSessionJob::CreateMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00437ED4
nn::pia::common::ExecuteResult nn::pia::session::CreateSessionJob::WaitCreateMesh()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (!m_CallContext.IsFinished()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
        Session::s_pInstance->m_pMeshLayerController->vf_0x14();
        return vf_0x24(m_CallContext.m_Result);
    }
    nn::Result result = m_CallContext.m_Result;
    Session* pSession = Session::s_pInstance;
    if (result.IsFailure()) {
        pSession->m_pMeshLayerController->vf_0x14();
        return vf_0x24(result);
    }
    pSession->m_State = 2;
    pSession->m_DisconnectState = 1;
    pSession->SetupStationIdsAsHost();
    if (pSession->IsUsingStationIdTable()) {
        pSession->m_Unknown0xB2 = false;
        pSession->m_pStationIdStatusTable->NotifyStations();
    }
    vf_0x20();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00438008
nn::pia::common::ExecuteResult nn::pia::session::CreateSessionJob::CompleteFailure()
{
    Session::s_pInstance->m_pMeshLayerController->vf_0x14();
    if (m_Result == common::RESULT_INVALID_STATE || m_Result == common::RESULT_NOT_IN_SESSION) {
        Session::s_pInstance->SetDisconnected();
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
    } else {
        m_pCallContext->SignalFailure(m_Result);
    }
    m_pCallContext = nullptr;
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(false);
    common::g_SessionBeginMonitoringContent.Cleanup();
    common::g_SessionStateMonitoringContent.Cleanup();
    Cleanup();
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x004380D0
nn::pia::common::ExecuteResult nn::pia::session::CreateSessionJob::CompleteProcess()
{
    m_pCallContext->SignalSuccess(nn::Result());
    m_pCallContext = nullptr;
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(false);
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x0043811C (name is ours)
void nn::pia::session::CreateSessionJob::SetSessionState(u8 state)
{
    Session::s_pInstance->m_State = state;
}

// 0x00438130 (name is ours)
void nn::pia::session::CreateSessionJob::SetSessionDisconnectState(u8 state)
{
    Session::s_pInstance->m_DisconnectState = state;
}

// 0x00438148 (name is ours)
void nn::pia::session::CreateSessionJob::Cleanup()
{
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
    m_Result = nn::Result();
}

// 0x00438198 (name is ours)
nn::Result nn::pia::session::CreateSessionJob::Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::session::CreateSessionSetting* pSetting)
{
    if (!common::IsValidPointer(pCallContext) || !common::IsValidPointer(pSetting)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    nn::Result result = vf_0x1C(pSetting);
    if (result.IsFailure()) {
        return result;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    Reset(true);
    common::g_SessionBeginMonitoringContent.m_Unknown0x270 = 0xFFFF;
    common::g_SessionBeginMonitoringContent.m_Unknown0x272 = 0xFFFF;
    return nn::Result();
}

// 0x00438248
nn::pia::session::CreateSessionJob::CreateSessionJob() : m_pCallContext(nullptr), m_Result(common::RESULT_NOT_SET)
{
}

// 0x004382A4
// 0x0043827C (deleting dtor)
nn::pia::session::CreateSessionJob::~CreateSessionJob()
{
    // empty (in the original too)
}

// 0x007338F4
void nn::pia::session::CreateSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
