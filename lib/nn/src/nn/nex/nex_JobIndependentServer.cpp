#include "nn/nex/nex_JobIndependentServer.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/friends/CTR/detail/detail_Api.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_ErrorCodeConverter.h"
#include "nn/nex/nex_IndependentServer.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results of the job (the names are ours)
const int RESULT_TIMEOUT = static_cast<int>(0x8001000B);
const int RESULT_REQUEST_FAILED = static_cast<int>(0x8001000C);
const int RESULT_RESTRICTED = static_cast<int>(0x80650013);
const int RESULT_AUTHENTICATION_REJECTED = static_cast<int>(0x80680001);
const int RESULT_FRIENDS_SERVER_ERROR = static_cast<int>(0x80680003);
const int RESULT_FRIENDS_SERVER_ERROR_260 = static_cast<int>(0x80680004);
const int RESULT_FRIENDS_SERVER_ERROR_261 = static_cast<int>(0x80680005);
// the friends server is busy with another request
const u32 RESULT_FRIENDS_BUSY = 0xC8A0C7F0;
// the result of the login of a console that is logged in already
const u32 RESULT_ALREADY_LOGGED_IN = 0xD8A0C4F5;
// the successful results of the friends server
const s32 HTTP_STATUS_OK = 200;
const s32 GAME_AUTHENTICATION_SUCCESS = 1;
const s32 SERVICE_LOCATOR_SUCCESS = 7;
// the descriptions of failed friends requests with their own results
const u32 DESCRIPTION_260 = 260;
const u32 DESCRIPTION_261 = 261;
// the description of a wait that timed out
const u32 DESCRIPTION_TIMEOUT = 0x3FE;
// the time between the tests of a waiting step (ms)
const int WAIT_TIME = 250;
// the service of the service locator request
const char* const SERVICE_LOCATOR_PREFIX = "0000";
// the network error codes of rejected authentications (by HTTP status / by result)
const u32 HTTP_ERROR_CODE_BASE = 23000;
const u32 RESPONSE_ERROR_CODE_BASE = 20000;
// the offset of the results of the service locator in the response result
const s32 SERVICE_LOCATOR_RESULT_OFFSET = 4000;

// the result of a failed friends request
inline int GetFriendsErrorResult(nn::Result result)
{
    u32 description = result.GetDescription();
    if (description == DESCRIPTION_260) {
        return RESULT_FRIENDS_SERVER_ERROR_260;
    }
    if (description == DESCRIPTION_261) {
        return RESULT_FRIENDS_SERVER_ERROR_261;
    }
    return RESULT_FRIENDS_SERVER_ERROR;
}
} // namespace

// 0x003968E4 (name is ours)
void nn::nex::JobIndependentServer::Start()
{
    nn::cfg::CTR::Initialize();
    bool isRestricted = nn::cfg::CTR::IsRestrictP2pInternet();
    nn::cfg::CTR::Finalize();
    if (m_ChecksRestriction && isRestricted) {
        CompleteJob(qResult(RESULT_RESTRICTED));
        return;
    }
    if (nn::friends::CTR::detail::GetMyPrincipalId() != 0) {
        SetStep(Step(&JobIndependentServer::RequestGameAuthentication, ""));
        return;
    }
    m_Event.Initialize(nn::os::RESET_TYPE_ONESHOT);
    m_Event.ClearSignal();
    if (nn::friends::CTR::detail::Login(&m_Event).IsFailure()) {
        CompleteJob(qResult(RESULT_REQUEST_FAILED));
        return;
    }
    SetStep(Step(&JobIndependentServer::WaitForLogin, ""));
}

// 0x00396A60 (name is ours)
void nn::nex::JobIndependentServer::WaitForLogin()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        CompleteJob(qResult(RESULT_TIMEOUT));
        return;
    }
    nn::Result waitResult = nn::svc::WaitSynchronization1(m_Event.GetHandle(), 0);
    if (waitResult.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(waitResult);
    }
    if (waitResult.GetDescription() == DESCRIPTION_TIMEOUT) {
        SetToWaiting(WAIT_TIME);
        return;
    }
    m_Event.Close();
    nn::Result result = nn::friends::CTR::detail::GetLastResponseResult();
    if (result.IsSuccess() || result.GetPrintableBits() == RESULT_ALREADY_LOGGED_IN) {
        SetStep(Step(&JobIndependentServer::RequestGameAuthentication, ""));
    } else {
        m_pServer->m_LastErrorCode = nn::friends::CTR::detail::ResultToErrorCode(result);
        CompleteJob(qResult(RESULT_REQUEST_FAILED));
    }
    if (nn::friends::CTR::detail::HasLoggedIn()) {
        nn::friends::CTR::detail::Logout();
    }
}

// 0x00396BDC (name is ours)
void nn::nex::JobIndependentServer::RequestGameAuthentication()
{
    wchar_t name[12] = {};
    m_Event.Initialize(nn::os::RESET_TYPE_ONESHOT);
    m_Event.ClearSignal();
    nn::Result result = nn::friends::CTR::detail::RequestGameAuthenticationWithoutLogin(&m_Event, m_GameServerId, name, 0, 0);
    if (result.IsSuccess()) {
        SetStep(Step(&JobIndependentServer::WaitForGameAuthentication, ""));
        return;
    }
    if (result.GetPrintableBits() != RESULT_FRIENDS_BUSY) {
        CompleteJob(qResult(RESULT_REQUEST_FAILED));
        return;
    }
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    SetToWaiting(WAIT_TIME);
    m_Event.Close();
}

// 0x00396D58 (name is ours)
void nn::nex::JobIndependentServer::RequestServiceLocator()
{
    char keyHash[12] = {};
    char service[8];
    std::memcpy(service, SERVICE_LOCATOR_PREFIX, sizeof(service));
    m_Event.Initialize(nn::os::RESET_TYPE_ONESHOT);
    m_Event.ClearSignal();
    std::strncpy(keyHash, m_pAccessKey, 8);
    nn::Result result = nn::friends::CTR::detail::RequestServiceLocatorWithoutLogin(&m_Event, m_GameServerId, keyHash, service, 0, 0);
    if (result.IsSuccess()) {
        SetStep(Step(&JobIndependentServer::WaitForServiceLocator, ""));
        return;
    }
    if (result.GetPrintableBits() != RESULT_FRIENDS_BUSY) {
        CompleteJob(qResult(RESULT_REQUEST_FAILED));
        return;
    }
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    SetToWaiting(WAIT_TIME);
    m_Event.Close();
}

// 0x00396EE4 (name is ours)
void nn::nex::JobIndependentServer::WaitForGameAuthentication()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        CompleteJob(qResult(RESULT_TIMEOUT));
        return;
    }
    nn::Result waitResult = nn::svc::WaitSynchronization1(m_Event.GetHandle(), 0);
    if (waitResult.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(waitResult);
    }
    if (waitResult.GetDescription() == DESCRIPTION_TIMEOUT) {
        SetToWaiting(WAIT_TIME);
        return;
    }
    nn::Result result = nn::friends::CTR::detail::GetLastResponseResult();
    if (result.IsFailure()) {
        CompleteJob(qResult(GetFriendsErrorResult(result)));
        return;
    }
    nnfriesndsGameAuthenticationData data;
    nn::friends::CTR::detail::GetGameAuthenticationData(&data);
    m_HttpStatusCode = data.httpStatusCode;
    m_ResponseResult = data.result;
    if (data.httpStatusCode != HTTP_STATUS_OK || data.result != GAME_AUTHENTICATION_SUCCESS) {
        CompleteJob(qResult(RESULT_AUTHENTICATION_REJECTED));
        return;
    }
    m_Event.Close();
    SetStep(Step(&JobIndependentServer::RequestServiceLocator, ""));
}

// 0x00397108 (name is ours)
void nn::nex::JobIndependentServer::WaitForServiceLocator()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        CompleteJob(qResult(RESULT_TIMEOUT));
        return;
    }
    nn::Result waitResult = nn::svc::WaitSynchronization1(m_Event.GetHandle(), 0);
    if (waitResult.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(waitResult);
    }
    if (waitResult.GetDescription() == DESCRIPTION_TIMEOUT) {
        SetToWaiting(WAIT_TIME);
        return;
    }
    nn::Result result = nn::friends::CTR::detail::GetLastResponseResult();
    if (result.IsFailure()) {
        CompleteJob(qResult(GetFriendsErrorResult(result)));
        return;
    }
    nnfriesndsServiceLocatorData data;
    nn::friends::CTR::detail::GetServiceLocatorData(&data);
    m_HttpStatusCode = data.httpStatusCode;
    m_ResponseResult = data.result;
    if (data.httpStatusCode != HTTP_STATUS_OK || data.result != SERVICE_LOCATOR_SUCCESS) {
        m_ResponseResult = data.result + SERVICE_LOCATOR_RESULT_OFFSET;
        CompleteJob(qResult(RESULT_AUTHENTICATION_REJECTED));
        return;
    }
    std::memcpy(m_pToken, data.serviceToken, sizeof(data.serviceToken));
    m_pToken[sizeof(data.serviceToken) - 1] = '\0';
    CompleteJob(qResult(qResult::SUCCESS));
}

// 0x00397320 (name is ours)
void nn::nex::JobIndependentServer::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        IndependentServer* pServer = m_pServer;
        if (pServer->m_LastErrorCode == 0) {
            if (result.m_Code == RESULT_AUTHENTICATION_REJECTED) {
                if (m_HttpStatusCode != HTTP_STATUS_OK) {
                    pServer->m_LastErrorCode = m_HttpStatusCode + HTTP_ERROR_CODE_BASE;
                } else {
                    pServer->m_LastErrorCode = m_ResponseResult + RESPONSE_ERROR_CODE_BASE;
                }
            } else {
                m_pServer->m_LastErrorCode = ErrorCodeConverter::ConvertToNetworkErrorCode(result);
            }
        }
        if (result) {
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    }
    if (m_State != STATE_COMPLETED) {
        SetToComplete();
    }
}

// 0x00397440 slot 0x00
// 0x003973FC (deleting dtor)
nn::nex::JobIndependentServer::~JobIndependentServer()
{
    // nothing to do (the event closes its handle)
}

} // namespace nex
} // namespace nn
