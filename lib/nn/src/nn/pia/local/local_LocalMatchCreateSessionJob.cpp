#include "nn/pia/local/local_LocalMatchCreateSessionJob.h"
#include <string.h>
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalCreateSessionSetting.h"
#include "nn/pia/local/local_LocalFacade.h"
#include "nn/pia/local/local_LocalMatchmakeSession.h"
#include "nn/pia/local/local_LocalNetwork.h"
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

// 0x00420C24
nn::Result nn::pia::local::LocalMatchCreateSessionJob::vf_0x1C(const nn::pia::session::CreateSessionSetting* pSetting)
{
    const LocalCreateSessionSetting* pLocalSetting = static_cast<const LocalCreateSessionSetting*>(pSetting);
    m_pSession = GetCurrentMatchmakeSession();
    m_pSession->SetCreateSessionSetting(pLocalSetting);

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
    SetStep(&LocalMatchCreateSessionJob::CreateLocalNetwork, "LocalMatchCreateSessionJob::CreateLocalNetwork");
    return nn::Result();
}

// 0x00420D14
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::CompleteFailure()
{
    m_pCallContext->SignalFailure(m_FailureResult);
    m_pCallContext = nullptr;
    return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
}

// 0x00420D40
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::StopLocalSession()
{
    LocalFacade::s_pInstance->Cleanup();
    SetStep(&LocalMatchCreateSessionJob::DestroyLocalNetwork, "LocalMatchCreateSessionJob::DestroyLocalNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00420DC8
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::StartLocalSession()
{
    LocalNetwork* pNetwork = LocalNetwork::s_pInstance;
    if (!pNetwork->IsHost() && !pNetwork->IsClient()) {
        m_pCallContext->SignalFailure(common::RESULT_NOT_IN_SESSION);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = LocalFacade::s_pInstance->Startup();
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&CreateSessionJob::MeshStartup, "CreateSessionJob::MeshStartup");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00420EB8
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::CreateLocalNetwork()
{
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    nn::Result result = m_pSession->CreateAsync(&m_CallContext);
    if (result.IsFailure()) {
        m_pCallContext->SignalFailure(result);
        m_pCallContext = nullptr;
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    SetStep(&LocalMatchCreateSessionJob::WaitCreateLocalNetwork, "LocalMatchCreateSessionJob::WaitCreateLocalNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00420F98
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::DestroyLocalNetwork()
{
    if (m_pSession->UnregisterAsync(&m_CallContext, 0).IsFailure()) {
        SetStep(&LocalMatchCreateSessionJob::CompleteFailure, "LocalMatchCreateSessionJob::CompleteFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    SetStep(&LocalMatchCreateSessionJob::WaitDestroyLocalNetwork, "LocalMatchCreateSessionJob::WaitDestroyLocalNetwork");
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00421084
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::WaitCreateLocalNetwork()
{
    if (m_pCallContext != nullptr && m_pCallContext->IsCancelRequested()) {
        Cleanup();
        return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
    }
    u32 sessionId = 0;
    if (m_pSession->IsCreateCompleted(&sessionId)) {
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE) {
            m_pCallContext->SignalFailure(m_CallContext.m_Result);
            m_pCallContext = nullptr;
            return common::ExecuteResult(common::ExecuteResult::STATE_SUCCESS);
        }
        if (m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS) {
            session::Session* pSession = session::Session::s_pInstance;
            pSession->m_SessionIds[pSession->m_CurrentIndex] = sessionId;
            SetSessionState(1);
            SetStep(&LocalMatchCreateSessionJob::StartLocalSession, "LocalMatchCreateSessionJob::StartLocalSession");
            return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
        }
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x004211B0
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::WaitDestroyLocalNetwork()
{
    if (m_pSession->IsUnregisterCompleted() &&
        (m_CallContext.m_State == common::CallContext::STATE_CALL_FAILURE || m_CallContext.m_State == common::CallContext::STATE_CALL_SUCCESS)) {
        SetStep(&LocalMatchCreateSessionJob::CompleteFailure, "LocalMatchCreateSessionJob::CompleteFailure");
        return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
    }
    return common::ExecuteResult(common::ExecuteResult::STATE_NEXT_DISPATCH);
}

// 0x00421258
void nn::pia::local::LocalMatchCreateSessionJob::vf_0x20()
{
    SetStep(&CreateSessionJob::CompleteProcess, "CreateSessionJob::CompleteProcess");
}

// 0x004212A4
nn::pia::common::ExecuteResult nn::pia::local::LocalMatchCreateSessionJob::vf_0x24(nn::Result result)
{
    m_FailureResult = result;
    SetStep(&LocalMatchCreateSessionJob::StopLocalSession, "LocalMatchCreateSessionJob::StopLocalSession");
    return common::ExecuteResult(common::ExecuteResult::STATE_CONTINUE);
}

// 0x00421314
nn::pia::local::LocalMatchCreateSessionJob::LocalMatchCreateSessionJob() : m_pSession(nullptr), m_FailureResult()
{
}

// 0x00421348
// 0x00421338 (deleting dtor)
nn::pia::local::LocalMatchCreateSessionJob::~LocalMatchCreateSessionJob()
{
    // empty (in the original too)
}

// 0x00438144
void nn::pia::local::LocalMatchCreateSessionJob::Cleanup()
{
    CreateSessionJob::Cleanup();
}

// 0x007316AC
void nn::pia::local::LocalMatchCreateSessionJob::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn
