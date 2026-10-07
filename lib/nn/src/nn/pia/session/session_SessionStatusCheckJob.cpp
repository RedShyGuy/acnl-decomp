#include "nn/pia/session/session_SessionStatusCheckJob.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/session/session_CommonMatchmakeSession.h"
#include "nn/pia/session/session_JointSessionJob.h"
#include "nn/pia/session/session_Mesh.h"
#include "nn/pia/session/session_MeshLayerController.h"
#include "nn/pia/session/session_Session.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace session {
// 0x00440720
nn::pia::common::ExecuteResult nn::pia::session::SessionStatusCheckJob::CheckSessionStatus()
{
    u8 networkStatus = Session::s_pInstance->m_pMeshLayerController->GetNetworkStatus();
    if (networkStatus != 1 && networkStatus != 0) {
        Session::s_pInstance->m_DisconnectState = networkStatus;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (transport::Transport::s_pInstance->m_StreamResult.IsFailure()) {
        Session::s_pInstance->m_DisconnectState = 3;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_pInstance->m_State == 2 && Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
        switch (Mesh::s_pInstance->GetDisconnectReason()) {
        case Mesh::DISCONNECT_REASON_NONE:
        case Mesh::DISCONNECT_REASON_1:
        case Mesh::DISCONNECT_REASON_BY_HOST:
        case Mesh::DISCONNECT_REASON_KICKOUT_4:
        case Mesh::DISCONNECT_REASON_KICKOUT_5:
        case Mesh::DISCONNECT_REASON_6:
        case Mesh::DISCONNECT_REASON_HOST_MIGRATION_FAILED:
        case Mesh::DISCONNECT_REASON_8:
        case Mesh::DISCONNECT_REASON_9:
            Session::s_pInstance->m_DisconnectState = 2;
            break;
        default:
            break;
        }
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    if (Session::s_GlobalSetting.m_Unknown0x1) {
        Session* pSession = Session::s_pInstance;
        CommonMatchmakeSession* pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
        if (Session::s_pInstance->GetUnknown0x100()) {
            Session::s_pInstance->m_Unknown0x100 = false;
            if (Mesh::IsLocalHost()) {
                pSession = Session::s_pInstance;
                pMatchmakeSession->vf_0x78(&m_CallContext, pSession->m_SessionIds[pSession->m_CurrentIndex]);
                m_IsChecking = true;
            }
        }
        if (m_IsChecking && pMatchmakeSession->vf_0x7C()) {
            m_IsChecking = false;
        }
        if (!Session::CheckJoinApproval(nullptr)) {
            pMatchmakeSession->vf_0xA0();
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00440904
nn::pia::common::ExecuteResult nn::pia::session::SessionStatusCheckJob::CheckSessionStatus4JointSession()
{
    Session* pSession = Session::s_pInstance;
    u8 networkStatus = pSession->m_pMeshLayerController->GetNetworkStatus();
    u8 state = pSession->m_State;
    if (networkStatus == 3) {
        pSession->m_DisconnectState = networkStatus;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUSPEND);
    }
    if (transport::Transport::s_pInstance->m_StreamResult.IsFailure()) {
        pSession->m_DisconnectState = 3;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUSPEND);
    }
    if (pSession->m_DisconnectState != 1) {
        return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
    }
    common::ExecuteResult result(common::ExecuteResult::STATE_NEXT_DISPATCH);
    u32 sessionId = pSession->m_SessionIds[pSession->m_CurrentIndex];
    u32 otherSessionId = pSession->m_SessionIds[pSession->m_CurrentIndex == 0 ? 1 : 0];
    if (state == 2 || state == 4) {
        if (Mesh::s_pInstance->CheckJoined() == common::RESULT_NOT_JOINED) {
            switch (Mesh::s_pInstance->GetDisconnectReason()) {
            case Mesh::DISCONNECT_REASON_LEAVE:
                break;
            case Mesh::DISCONNECT_REASON_NONE:
            case Mesh::DISCONNECT_REASON_1:
            case Mesh::DISCONNECT_REASON_BY_HOST:
            case Mesh::DISCONNECT_REASON_KICKOUT_4:
            case Mesh::DISCONNECT_REASON_KICKOUT_5:
            case Mesh::DISCONNECT_REASON_6:
            case Mesh::DISCONNECT_REASON_HOST_MIGRATION_FAILED:
            case Mesh::DISCONNECT_REASON_8:
            case Mesh::DISCONNECT_REASON_9:
                pSession->m_DisconnectState = 2;
                return result;
            default:
                pSession->m_DisconnectState = 3;
                result = common::ExecuteResult(common::ExecuteResult::STATE_SUSPEND);
                break;
            }
        } else if (state == 2) {
            // the network must not have other sessions
            if (sessionId == 0 || pSession->m_pMeshLayerController->HasOtherSession(sessionId, 0)) {
                pSession->m_DisconnectState = 2;
                return result;
            }
        } else if (state == 4) {
            if (sessionId == 0 || otherSessionId == 0 || pSession->m_pMeshLayerController->HasOtherSession(sessionId, otherSessionId)) {
                pSession->m_DisconnectState = 2;
                return result;
            }
        }
    } else if (state == 3) {
        JointSessionJob* pJointSessionJob = pSession->m_pJointSessionJob;
        if ((pJointSessionJob == nullptr || pJointSessionJob->m_Unknown0x9D == 0) && sessionId == 0 && otherSessionId == 0) {
            Session::s_pInstance->m_DisconnectState = 3;
            result = common::ExecuteResult(common::ExecuteResult::STATE_SUSPEND);
        }
    } else if (networkStatus == 2) {
        pSession->m_DisconnectState = 2;
        return result;
    }
    if (pSession->m_DisconnectState != 1) {
        return result;
    }
    if (!Session::s_GlobalSetting.m_Unknown0x1) {
        return result;
    }
    // the joint session checks the other matchmake session
    CommonMatchmakeSession* pMatchmakeSession;
    if (pSession->GetStatus() == Session::STATUS_JOINT) {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex == 0 ? 1 : 0];
    } else {
        pMatchmakeSession = pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex];
    }
    if (pSession->GetUnknown0x100()) {
        pSession->m_Unknown0x100 = false;
        if (Mesh::IsLocalHost() && !m_IsChecking) {
            if (pSession->GetStatus() == Session::STATUS_JOINT) {
                pMatchmakeSession->vf_0x78(&m_CallContext, pSession->GetJointSessionId());
            } else {
                pMatchmakeSession->vf_0x78(&m_CallContext, pSession->m_SessionIds[pSession->m_CurrentIndex]);
            }
            m_IsChecking = true;
        }
    }
    if (Mesh::IsLocalHost() && m_IsCheckRequested && !m_IsChecking) {
        if (pSession->GetStatus() == Session::STATUS_JOINT) {
            pMatchmakeSession->vf_0x78(&m_CallContext, pSession->GetJointSessionId());
            m_IsCheckRequested = false;
            m_IsChecking = true;
        } else if (Session::s_pInstance->GetStatus() == Session::STATUS_1) {
            pSession = Session::s_pInstance;
            pMatchmakeSession->vf_0x78(&m_CallContext, pSession->m_SessionIds[pSession->m_CurrentIndex]);
            m_IsCheckRequested = false;
            m_IsChecking = true;
        }
    }
    if (Session::s_pInstance->GetStatus() == Session::STATUS_2 && !m_IsCheckRequested) {
        m_IsCheckRequested = true;
    }
    if (m_IsChecking && pMatchmakeSession->vf_0x7C()) {
        m_IsChecking = false;
    }
    // the joint session has its own identification (Session::m_Unknown0x101)
    const transport::Station::IdentificationInfo* pInfo =
        reinterpret_cast<const transport::Station::IdentificationInfo*>(Session::s_pInstance->GetUnknown0x101());
    if (!Session::CheckJoinApproval(pInfo)) {
        pMatchmakeSession->vf_0xA0();
    }
    return result;
}

// 0x00440D38 (name is ours)
void nn::pia::session::SessionStatusCheckJob::Cleanup()
{
    if (m_CallContext.m_State == common::CallContext::STATE_CALL_IN_PROGRESS) {
        m_CallContext.SignalCancel();
    }
    m_CallContext.Reset();
    m_IsChecking = false;
    CancelCleanup();
    Reset(true);
}

// 0x00440D88 (name is ours)
nn::Result nn::pia::session::SessionStatusCheckJob::Startup()
{
    if (Session::s_pInstance->m_pMeshLayerController->GetNetworkStatus() == 3) {
        return common::RESULT_NOT_IN_SESSION;
    }
    Reset(true);
    if (Session::s_pInstance->IsUsingStationIdTable()) {
        SetStep(&SessionStatusCheckJob::CheckSessionStatus4JointSession, "SessionStatusCheckJob::CheckSessionStatus4JointSession");
    } else {
        SetStep(&SessionStatusCheckJob::CheckSessionStatus, "SessionStatusCheckJob::CheckSessionStatus");
    }
    return nn::Result();
}

// 0x00440E80
nn::pia::session::SessionStatusCheckJob::SessionStatusCheckJob() : m_IsChecking(false), m_IsCheckRequested(false)
{
}

// 0x00440ED0
// 0x00440EAC (deleting dtor)
nn::pia::session::SessionStatusCheckJob::~SessionStatusCheckJob()
{
    // empty (in the original too)
}

// 0x00734064
void nn::pia::session::SessionStatusCheckJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace session
} // namespace pia
} // namespace nn
