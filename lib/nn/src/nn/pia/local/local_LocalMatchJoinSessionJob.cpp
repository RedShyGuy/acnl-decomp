#include "nn/pia/local/local_LocalMatchJoinSessionJob.h"
#include <string.h>
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalFacade.h"
#include "nn/pia/local/local_LocalJoinSessionSetting.h"
#include "nn/pia/local/local_LocalMatchmakeSession.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalSessionInfo.h"
#include "nn/pia/session/session_Session.h"

namespace nn {
namespace pia {
namespace local {
namespace {
inline LocalMatchmakeSession* GetCurrentMatchmakeSession()
{
    session::Session* pSession = session::Session::s_pInstance;
    return static_cast<LocalMatchmakeSession*>(pSession->m_pMatchmakeSessions[pSession->m_CurrentIndex]);
}
} // namespace

// 0x0041F7E0
nn::Result nn::pia::local::LocalMatchJoinSessionJob::vf_0x30(const nn::pia::session::JoinSessionSetting* pSetting)
{
    const LocalJoinSessionSetting* pLocalSetting = static_cast<const LocalJoinSessionSetting*>(pSetting);
    m_pSession = GetCurrentMatchmakeSession();
    const char* pPassphrase = pLocalSetting->GetPassphrase();
    const LocalSessionInfo* pInfo = static_cast<const LocalSessionInfo*>(pLocalSetting->GetSessionInfo());
    nn::Result result = m_pSession->SetJoinSetting(pInfo, pPassphrase, pLocalSetting->m_PassphraseSize);
    if (result.IsFailure()) {
        return result;
    }

    // the application data is the key of the signature
    u32 size = pLocalSetting->m_ApplicationDataSize;
    if (size > SIGNATURE_KEY_SIZE) {
        size = SIGNATURE_KEY_SIZE;
    }
    LocalMatchmakeSession* pMatchmakeSession = GetCurrentMatchmakeSession();
    if (size != 0) {
        pMatchmakeSession->m_SignatureSetting.Set(common::SignatureSetting::MODE_HMAC_MD5, pMatchmakeSession->m_SignatureSetting.m_KeyBuffer, SIGNATURE_KEY_SIZE);
    } else {
        pMatchmakeSession->m_SignatureSetting.Set(common::SignatureSetting::MODE_NONE, pMatchmakeSession->m_SignatureSetting.m_KeyBuffer, SIGNATURE_KEY_SIZE);
    }
    memcpy(pMatchmakeSession->m_SignatureSetting.m_KeyBuffer, pLocalSetting->m_ApplicationData, size);
    SetStep(&LocalMatchJoinSessionJob::JoinMatchmakeSession, "LocalMatchJoinSessionJob::JoinMatchmakeSession");
    return nn::Result();
}

// 0x0041F908
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::WaitForCancel()
{
    if (GetCurrentMatchmakeSession()->IsJoinCompleted(nullptr, nullptr, nullptr)) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041F988
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::WaitJoinMatchmake()
{
    if (IsCancelRequested()) {
        if (!LocalNetwork::s_pInstance->IsConnectNetworkAsyncCompleted() && LocalNetwork::s_pInstance->CancelConnectNetworkAsync().IsFailure()) {
            m_pCallContext->SignalCancel();
            m_pCallContext = nullptr;
            Cleanup();
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        SetStep(&LocalMatchJoinSessionJob::WaitForCancel, "LocalMatchJoinSessionJob::WaitForCancel");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    if (GetCurrentMatchmakeSession()->IsJoinCompleted(nullptr, nullptr, nullptr)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_pCallContext->SignalFailure(m_CallContext.m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            session::Session* pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex] = m_SessionId;
            SetSessionState(1);
            m_CallContext.Reset();
            LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
            if (!pNetwork->IsHost() && !pNetwork->IsClient()) {
                m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
                m_pCallContext = nullptr;
                return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
            }
            nn::Result result = LocalFacade::s_pInstance->Startup();
            if (result.IsSuccess()) {
                result = LocalFacade::s_pInstance->GetHostStationConnectionInfo(&m_ConnectionInfo);
            }
            if (result.IsFailure()) {
                m_pCallContext->SignalFailure(result);
                m_pCallContext = nullptr;
                return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
            }
            SetStep(&JoinSessionJob::MeshStartup, "JoinSessionJob::MeshStartup");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041FBF0
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::WaitLeaveMatchmake()
{
    if (GetCurrentMatchmakeSession()->IsLeaveCompleted()) {
        // (a failure was only logged)
        m_CallContext.Reset();
        SetStep(&JoinSessionJob::CompleteFailure, "LocalMatchJoinSessionJob::CompleteFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041FCE0
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::JoinMatchmakeSession()
{
    if (IsCancelRequested()) {
        m_pCallContext->SignalCancel();
        m_pCallContext = nullptr;
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = GetCurrentMatchmakeSession()->JoinAsync(&m_CallContext, m_SessionId);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&LocalMatchJoinSessionJob::WaitJoinMatchmake, "LocalMatchJoinSessionJob::WaitJoinMatchmake");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041FDF0
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::LeaveMatchmakeSession()
{
    if (GetCurrentMatchmakeSession()->LeaveAsync(&m_CallContext, m_SessionId).IsFailure()) {
        SetStep(&JoinSessionJob::CompleteFailure, "LocalMatchJoinSessionJob::CompleteFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&LocalMatchJoinSessionJob::WaitLeaveMatchmake, "LocalMatchJoinSessionJob::WaitLeaveMatchmake");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x0041FEEC
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::vf_0x40()
{
    SetStep(&LocalMatchJoinSessionJob::LeaveMatchmakeSession, "LocalMatchJoinSessionJob::LeaveMatchmakeSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041FF54
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::vf_0x3C()
{
    m_Result = common::RESULT_CANCELED;
    SetStep(&JoinSessionJob::MeshCleanup, "LocalMatchJoinSessionJob::MeshCleanup");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x0041FFC0
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchJoinSessionJob::vf_0x38(nn::Result result)
{
    m_Result = result;
    SetStep(&JoinSessionJob::MeshCleanup, "LocalMatchJoinSessionJob::MeshCleanup");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00420028
nn::pia::local::LocalMatchJoinSessionJob::LocalMatchJoinSessionJob() : m_pSession(nullptr)
{
}

// 0x00420058
// 0x00420048 (deleting dtor)
nn::pia::local::LocalMatchJoinSessionJob::~LocalMatchJoinSessionJob()
{
    // empty (in the original too)
}

// 0x00731698
void nn::pia::local::LocalMatchJoinSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
