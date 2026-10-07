#include "nn/pia/session/session_DestroySessionJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace session {
// 0x004389D4
nn::pia::common::ExecuteResult nn::pia::session::DestroySessionJob::DestroyMesh()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (m_pCallContext != nullptr && m_pCallContext->m_State == common::CallContext::STATE_CALL_CANCEL) {
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Mesh::s_pInstance->m_IsMonitoringDataSent = true;
    if (m_IsJointSessionJobRunning) {
        vf_0x20();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    nn::Result result = Mesh::s_pInstance->DestroyMesh(&m_CallContext);
    if (result.IsFailure()) {
        m_Result = result;
        vf_0x20();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&DestroySessionJob::WaitDestroyMesh, "DestroySessionJob::WaitDestroyMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00438B18
nn::pia::common::ExecuteResult nn::pia::session::DestroySessionJob::MeshCleanup()
{
    s32 msec = (common::Scheduler::s_pInstance->m_DispatchTime - m_StartTime).m_Tick / common::TimeSpan::GetTicksPerMSec().m_Tick;
    if (msec >= 0) {
        common::g_SessionStateMonitoringContent.m_Unknown0x3D0 = msec;
    }
    Mesh::s_pInstance->m_IsMonitoringDataSent = false;
    Mesh::s_pInstance->SendMonitoringData(true);
    Session::s_pInstance->m_pMeshLayerController->vf_0x14();
    Session::s_pInstance->ClearStatus(false);
    vf_0x24();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00438BD4
nn::pia::common::ExecuteResult nn::pia::session::DestroySessionJob::WaitDestroyMesh()
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
        m_Result = m_CallContext.m_Result;
    }
    vf_0x20();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00438C6C
nn::pia::common::ExecuteResult nn::pia::session::DestroySessionJob::WaitForcedTerminatingOfJointSessionJob()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob != nullptr && pJointSessionJob->IsRunning()) {
        m_IsJointSessionJobRunning = true;
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    SetStep(&DestroySessionJob::DestroyMesh, "DestroySessionJob::DestroyMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00438D3C (name is ours)
void nn::pia::session::DestroySessionJob::Cleanup()
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

// 0x00438D8C (name is ours)
nn::Result nn::pia::session::DestroySessionJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    Session* pSession = Session::s_pInstance;
    if (pSession->m_IsHostMigrationEnabled) {
        return common::RESULT_INVALID_STATE;
    }
    if (pSession->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    nn::Result result = vf_0x1C();
    if (result.IsFailure()) {
        return result;
    }
    SetStep(&DestroySessionJob::WaitForcedTerminatingOfJointSessionJob, "DestroySessionJob::WaitForcedTerminatingOfJointSessionJob");
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_Result = nn::Result();
    m_IsJointSessionJobRunning = false;
    Reset(true);
    common::Time now;
    now.SetNow();
    m_StartTime = now;
    // the joint session job stops
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob != nullptr) {
        pJointSessionJob->m_IsFailed = true;
        pJointSessionJob->m_FailureResult = common::RESULT_CANCELED;
    }
    return nn::Result();
}

// 0x00438EC8
nn::pia::session::DestroySessionJob::DestroySessionJob() : m_pCallContext(nullptr), m_Result(common::RESULT_NOT_SET)
{
}

// 0x00438F34
// 0x00438F0C (deleting dtor)
nn::pia::session::DestroySessionJob::~DestroySessionJob()
{
    // empty (in the original too)
}

// 0x007338FC
void nn::pia::session::DestroySessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
