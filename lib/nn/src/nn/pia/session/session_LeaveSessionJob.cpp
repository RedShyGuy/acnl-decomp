#include "nn/pia/session/session_LeaveSessionJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"

namespace nn {
namespace pia {
namespace session {
// 0x00433360
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::MeshCleanup()
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

// 0x0043341C
void nn::pia::session::LeaveSessionJob::vf_0x3C()
{
    // empty (in the original too)
}

// 0x00433420
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::WaitLeaveMesh()
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
    vf_0x1C();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004334B8
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::WaitHostMigrated()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (!(common::Scheduler::s_pInstance->m_DispatchTime < m_HostMigrationDeadline)) {
        vf_0x30();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the session (or the other one of a joint session) has a new owner when the local
    // station is the only one left
    Session* pSession = Session::s_pInstance;
    u32 otherIndex = pSession->m_CurrentIndex == 0 ? 1 : 0;
    if (pSession->m_SessionIds[otherIndex] != 0) {
        if (pSession->m_pMatchmakeSessions[otherIndex]->vf_0x88()) {
            if (Session::s_pInstance->m_State == 4 && Session::s_pInstance->GetMeshLayerControllerValue() == 1) {
                vf_0x1C();
                return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
            }
            if (Session::s_pInstance->m_State == 3 && Session::s_pInstance->GetStationNum() == 1) {
                vf_0x1C();
                return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
            }
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
        pSession = Session::s_pInstance;
        if (pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->vf_0x88()) {
            Session::s_pInstance->m_pStationIdStatusTable->GetStationNum(Session::s_pInstance->m_SessionIds[Session::s_pInstance->m_CurrentIndex]);
        }
        vf_0x1C();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    u32 index = pSession->m_CurrentIndex;
    if (pSession->m_SessionIds[index] == 0 || !pSession->m_pMatchmakeSessions[index]->vf_0x88()) {
        vf_0x1C();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (Session::s_pInstance->IsUsingStationIdTable()) {
        if (Session::s_pInstance->m_State == 2 && Session::s_pInstance->GetMeshLayerControllerValue() == 1) {
            vf_0x1C();
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
        if (Session::s_pInstance->m_State == 3 && Session::s_pInstance->GetStationNum() == 1) {
            vf_0x1C();
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004336CC
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::LeaveBufferMatchmakeSession()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    u32 otherIndex = pSession->m_CurrentIndex == 0 ? 1 : 0;
    CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[otherIndex];
    if (pSession->m_SessionIds[otherIndex] == 0) {
        SetStep(&LeaveSessionJob::LeaveCurrentMatchmakeSession, "LeaveSessionJob::LeaveCurrentMatchmakeSession");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    nn::Result result = pMatchmakeSession->LeaveAsync(&m_CallContext, pSession->m_SessionIds[otherIndex]);
    if (result.IsFailure()) {
        m_Result = result;
        Session* pSession2 = Session::s_pInstance;
        pSession2->m_SessionIds[pSession2->m_CurrentIndex == 0 ? 1 : 0] = 0;
        vf_0x1C();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&LeaveSessionJob::WaitLeaveBufferMatchmakeSession, "LeaveSessionJob::WaitLeaveBufferMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00433868
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::LeaveCurrentMatchmakeSession()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    if (vf_0x2C()) {
        vf_0x20();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    pSession = Session::s_pInstance;
    nn::Result result = pMatchmakeSession->LeaveAsync(&m_CallContext, pSession->m_SessionIds[pSession->m_CurrentIndex]);
    if (result.IsFailure()) {
        m_Result = result;
        vf_0x28();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&LeaveSessionJob::WaitLeaveCurrentMatchmakeSession, "LeaveSessionJob::WaitLeaveCurrentMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004339A8
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::WaitLeaveMeshWithHostMigration()
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
    if (m_IsLastStation) {
        vf_0x1C();
    } else {
        const common::Time& now = common::Scheduler::s_pInstance->m_DispatchTime;
        s32 waitMSec = GetHostMigrationWaitMSec();
        m_HostMigrationDeadline = now + common::TimeSpan(common::TimeSpan::GetTicksPerMSec().m_Tick * waitMSec);
        SetStep(&LeaveSessionJob::WaitHostMigrated, "LeaveSessionJob::WaitHostMigrated");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00433AE4
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::WaitLeaveBufferMatchmakeSession()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]->IsLeaveCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE && m_CallContext.m_Result != common::RESULT_MATCHMAKE_SESSION_GONE) {
        m_Result = m_CallContext.m_Result;
    }
    pSession = Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = 0;
    vf_0x1C();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00433BD0
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::WaitLeaveCurrentMatchmakeSession()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    Session* pSession = Session::s_pInstance;
    if (!pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]->IsLeaveCompleted()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    pSession = Session::s_pInstance;
    pSession->m_SessionIds[pSession->m_CurrentIndex] = 0;
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE && m_CallContext.m_Result != common::RESULT_MATCHMAKE_SESSION_GONE) {
        m_Result = m_CallContext.m_Result;
    }
    vf_0x20();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00433CA4
void nn::pia::session::LeaveSessionJob::vf_0x38()
{
    Session::s_pInstance->SetDisconnected();
}

// 0x00433CB4
void nn::pia::session::LeaveSessionJob::vf_0x30()
{
    vf_0x1C();
}

// 0x00433CC0
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::WaitForcedTerminatingOfJointSessionJob()
{
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob != nullptr && pJointSessionJob->IsRunning()) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    // the host of both sessions of a joint session hands them over first
    if (Session::s_pInstance->IsUsingStationIdTable() && Session::s_pInstance->m_IsHostMigrationEnabled && Session::s_pInstance->IsHostOfBothSessions() &&
        Session::s_pInstance->m_State == 4) {
        vf_0x18();
    } else {
        SetStep(&LeaveSessionJob::LeaveMesh, "LeaveSessionJob::LeaveMesh");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00433DD4
void nn::pia::session::LeaveSessionJob::vf_0x28()
{
    SetStep(&LeaveSessionJob::MeshCleanup, "LeaveSessionJob::MeshCleanup");
}

// 0x00433E1C
void nn::pia::session::LeaveSessionJob::vf_0x18()
{
    SetStep(&LeaveSessionJob::LeaveMesh, "LeaveSessionJob::LeaveMesh");
}

// 0x00433E60 (name is ours)
void nn::pia::session::LeaveSessionJob::Cleanup()
{
    vf_0x3C();
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
    m_IsLastStation = false;
}

// 0x00433EC4 (name is ours)
nn::Result nn::pia::session::LeaveSessionJob::Startup(nn::pia::common::CallContext* pCallContext)
{
    if (m_pCallContext != nullptr) {
        return common::RESULT_INVALID_STATE;
    }
    // the host without host migration destroys the session instead
    Mesh* pMesh = Mesh::s_pInstance;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex && !pMesh->m_IsHostMigrationEnabled) {
        return common::RESULT_INVALID_STATE;
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_DISCONNECTED_5) {
        return common::RESULT_NOT_IN_SESSION;
    }
    m_pCallContext = pCallContext;
    pCallContext->InitiateCall();
    m_Result = nn::Result();
    m_IsLastStation = false;
    Reset(true);
    common::Time now;
    now.SetNow();
    m_StartTime = now;
    JointSessionJob* pJointSessionJob = Session::s_pInstance->m_pJointSessionJob;
    if (pJointSessionJob != nullptr) {
        pJointSessionJob->m_IsFailed = true;
        pJointSessionJob->m_FailureResult = common::RESULT_CANCELED;
    }
    SetStep(&LeaveSessionJob::WaitForcedTerminatingOfJointSessionJob, "LeaveSessionJob::WaitForcedTerminatingOfJointSessionJob");
    return nn::Result();
}

// 0x00434014
nn::pia::common::ExecuteResult nn::pia::session::LeaveSessionJob::LeaveMesh()
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
    Mesh* pMesh = Mesh::s_pInstance;
    nn::Result result;
    if (pMesh->m_LocalStationIndex <= STATION_INDEX_MAX && pMesh->m_LocalStationIndex == pMesh->m_HostStationIndex && Session::s_pInstance->m_IsHostMigrationEnabled) {
        // the host hands the mesh over
        if (Session::s_pInstance->GetStationNum() == 1) {
            m_IsLastStation = true;
        }
        result = Mesh::s_pInstance->LeaveMeshWithHostMigration(&m_CallContext);
        if (result.IsSuccess()) {
            SetStep(&LeaveSessionJob::WaitLeaveMeshWithHostMigration, "LeaveSessionJob::WaitLeaveMeshWithHostMigration");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
    } else {
        result = pMesh->LeaveMesh(&m_CallContext);
        if (result.IsSuccess()) {
            SetStep(&LeaveSessionJob::WaitLeaveMesh, "LeaveSessionJob::WaitLeaveMesh");
            return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
        }
    }
    m_Result = result;
    vf_0x1C();
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x004341C8
nn::pia::session::LeaveSessionJob::LeaveSessionJob() : m_pCallContext(nullptr), m_Result(common::RESULT_NOT_SET)
{
}

// 0x0043423C
// 0x00434214 (deleting dtor)
nn::pia::session::LeaveSessionJob::~LeaveSessionJob()
{
    // empty (in the original too)
}

// 0x007338D0
s32 nn::pia::session::LeaveSessionJob::GetHostMigrationWaitMSec()
{
    return 0;
}

// 0x007338D8
void nn::pia::session::LeaveSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
