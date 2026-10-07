#include "nn/pia/inet/inet_NexMatchLeaveSessionJob.h"
#include <string.h>
#include "nn/pia/common/common_Result.h"
#include "nn/pia/inet/inet_NexMatchMeshLayerController.h"
#include "nn/pia/inet/inet_NexMatchmakeSession.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/session/session_StationIdStatusTable.h"
#include "nn/pia/transport/transport_StationIdTable.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace inet {
// 0x00404080
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchLeaveSessionJob::CompleteProcess()
{
    if (m_Result.IsFailure()) {
        if (m_Result == common::RESULT_INVALID_STATE || m_Result == common::RESULT_NOT_IN_SESSION) {
            vf_0x38();
            m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        } else {
            m_pCallContext->SignalFailure(m_Result);
        }
    } else {
        m_pCallContext->SignalSuccess(nn::Result());
    }
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00404104
void nn::pia::inet::NexMatchLeaveSessionJob::vf_0x24()
{
    SetStep(&NexMatchLeaveSessionJob::CompleteProcess, "NexMatchLeaveSessionJob::CompleteProcess");
}

// 0x00404158
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchLeaveSessionJob::GetMatchmakeSessionOwners()
{
    session::Session* pSession = session::Session::s_pInstance;
    for (u8 i = 0; i < m_SessionNum; i++) {
        if (m_SessionIds[i] != pSession->m_SessionIds[pSession->m_CurrentIndex] && !m_IsOwnerAsked[i]) {
            m_IsOwnerAsked[i] = true;
            NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
            nn::Result result = pMatchmakeSession->GetOwnerAsync(&m_CallContext, m_SessionIds[i]);
            if (result.IsSuccess()) {
                SetStep(&NexMatchLeaveSessionJob::WaitGetMatchmakeSessionOwners, "NexMatchLeaveSessionJob::WaitGetMatchmakeSessionOwners");
                return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
            }
            if (result == common::RESULT_UNREGISTER_FAILED) {
                m_Result = result;
            }
        }
    }
    SetStep(&LeaveSessionJob::LeaveMesh, "LeaveSessionJob::LeaveMesh");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// (inline; name is ours)
DECOMP_ALWAYS_INLINE bool nn::pia::inet::NexMatchLeaveSessionJob::SetJoinedSessionAsOther()
{
    NexMatchMeshLayerController* pController = static_cast<NexMatchMeshLayerController*>(session::Session::s_pInstance->m_pMeshLayerController);
    if (pController == nullptr) {
        return false;
    }
    for (u32 i = 0; i < pController->GetSessionEntryNum(); i++) {
        u32 sessionId = pController->GetSessionId(i);
        if (sessionId != 0) {
            session::Session* pSession = session::Session::s_pInstance;
            if (pSession->m_SessionIds[pSession->m_CurrentIndex] != sessionId) {
                pSession = session::Session::s_pInstance;
                pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] = sessionId;
                return true;
            }
        }
    }
    return false;
}

// 0x004042A4
void nn::pia::inet::NexMatchLeaveSessionJob::vf_0x1C()
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0 || SetJoinedSessionAsOther()) {
        SetStep(&LeaveSessionJob::LeaveBufferMatchmakeSession, "LeaveSessionJob::LeaveBufferMatchmakeSession");
        return;
    }
    SetStep(&LeaveSessionJob::LeaveCurrentMatchmakeSession, "LeaveSessionJob::LeaveCurrentMatchmakeSession");
}

// 0x004043FC
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchLeaveSessionJob::MigrateMatchmakeSessionOwner()
{
    session::Session* pSession = session::Session::s_pInstance;
    if (!pSession->IsUsingStationIdTable() || pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] == 0 ||
        (pSession->m_State != 4 && pSession->m_State != 3)) {
        vf_0x1C();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    // the stations of the current session become the candidates for the owner of the other one
    session::StationIdStatusTable* pStatusTable = pSession->m_pStationIdStatusTable;
    u32 principalIds[STATION_INDEX_MAX];
    u32 principalNum = 0;
    typedef common::ObjList<transport::StationIdTable::Entry> List;
    List& list = transport::Transport::s_pInstance->m_pStationIdTable->m_List;
    for (List::Node* node = list.Begin(); node != list.End(); node = List::Advance(node)) {
        u32 sessionId;
        if (pStatusTable->GetSessionId(node->m_Value.m_StationId, &sessionId) &&
            pSession->m_SessionIds[pSession->m_CurrentIndex] == sessionId) {
            principalIds[principalNum] = node->m_Value.m_StationId.m_Low;
            principalNum++;
        }
    }
    u32 otherIndex = pSession->m_CurrentIndex == 0 ? 1 : 0;
    NexMatchmakeSession* pOther = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[otherIndex]);
    nn::Result result = pOther->MigrateOwnerAsync(&m_CallContext, pSession->m_SessionIds[otherIndex], principalIds, principalNum);
    if (result.IsFailure()) {
        if (result == common::RESULT_UNREGISTER_FAILED) {
            m_Result = result;
        }
        vf_0x1C();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&NexMatchLeaveSessionJob::WaitMigrateMatchmakeSessionOwner, "NexMatchLeaveSessionJob::WaitMigrateMatchmakeSessionOwner");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004045E0
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchLeaveSessionJob::WaitGetMatchmakeSessionOwners()
{
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pMatchmakeSession = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
    if (pSession->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    u32 ownerPrincipalId;
    if (pMatchmakeSession->IsGetOwnerCompleted(&ownerPrincipalId)) {
        if (m_CallContext.m_Result.IsFailure()) {
            SetStep(&NexMatchLeaveSessionJob::GetMatchmakeSessionOwners, "NexMatchLeaveSessionJob::GetMatchmakeSessionOwners");
        }
        // the owner of the other session stays
        pSession->m_pStationIdStatusTable->SetUnknown0x10(StationId(ownerPrincipalId, 0), 1);
        SetStep(&NexMatchLeaveSessionJob::GetMatchmakeSessionOwners, "NexMatchLeaveSessionJob::GetMatchmakeSessionOwners");
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0040470C
nn::pia::common::ExecuteResult nn::pia::inet::NexMatchLeaveSessionJob::WaitMigrateMatchmakeSessionOwner()
{
    session::Session* pSession = session::Session::s_pInstance;
    NexMatchmakeSession* pOther = static_cast<NexMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0]);
    if (pSession->GetStatus() == session::Session::STATUS_DISCONNECTED_5) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (pOther->IsMigrateOwnerCompleted()) {
        vf_0x1C();
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004047B8
void nn::pia::inet::NexMatchLeaveSessionJob::vf_0x30()
{
    SetStep(&NexMatchLeaveSessionJob::MigrateMatchmakeSessionOwner, "NexMatchLeaveSessionJob::MigrateMatchmakeSessionOwner");
}

// 0x00404818
bool nn::pia::inet::NexMatchLeaveSessionJob::vf_0x2C()
{
    session::Session* pSession = session::Session::s_pInstance;
    return pSession->m_SessionIds[pSession->m_CurrentIndex] == 0;
}

// 0x00404840
void nn::pia::inet::NexMatchLeaveSessionJob::vf_0x20()
{
    session::Session* pSession = session::Session::s_pInstance;
    if (pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0] != 0 || SetJoinedSessionAsOther()) {
        SetStep(&LeaveSessionJob::LeaveBufferMatchmakeSession, "LeaveSessionJob::LeaveBufferMatchmakeSession");
        return;
    }
    SetStep(&LeaveSessionJob::MeshCleanup, "LeaveSessionJob::MeshCleanup");
}

// 0x0040498C
void nn::pia::inet::NexMatchLeaveSessionJob::vf_0x18()
{
    m_SessionNum = session::Session::s_pInstance->m_pStationIdStatusTable->GetSessionIds(m_SessionIds);
    for (s32 i = 0; i < m_SessionNum; i++) {
        m_IsOwnerAsked[i] = false;
    }
    SetStep(&NexMatchLeaveSessionJob::GetMatchmakeSessionOwners, "NexMatchLeaveSessionJob::GetMatchmakeSessionOwners");
}

// 0x00404A54
nn::pia::inet::NexMatchLeaveSessionJob::NexMatchLeaveSessionJob() : m_SessionNum(0)
{
    memset(m_SessionIds, 0, sizeof(m_SessionIds));
    memset(m_IsOwnerAsked, 0, sizeof(m_IsOwnerAsked));
}

// 0x00434238
// 0x00404AB4 (deleting dtor)
nn::pia::inet::NexMatchLeaveSessionJob::~NexMatchLeaveSessionJob()
{
    // empty (in the original too)
}

// 0x00134CE4
void nn::pia::inet::NexMatchLeaveSessionJob::vf_0x3C()
{
    m_SessionNum = 0;
    memset(m_SessionIds, 0, sizeof(m_SessionIds));
    memset(m_IsOwnerAsked, 0, sizeof(m_IsOwnerAsked));
}

// 0x0072F51C
s32 nn::pia::inet::NexMatchLeaveSessionJob::GetHostMigrationWaitMSec()
{
    return HOST_MIGRATION_WAIT_MSEC;
}

// 0x0072F528
void nn::pia::inet::NexMatchLeaveSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn
