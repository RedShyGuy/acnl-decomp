#include "nn/nex/nex_JobCTRLogin.h"
#include "nn/friends/CTR/detail/detail_Api.h"
#include "nn/nex/nex_NgsFacade.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_RendezVous.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results of the login (the names are ours)
const int RESULT_TIMEOUT = static_cast<int>(0x8001000B);
const int RESULT_REQUEST_FAILED = static_cast<int>(0x8001000C);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_AUTHENTICATION_REJECTED = static_cast<int>(0x80680001);
const int RESULT_FRIENDS_SERVER_ERROR = static_cast<int>(0x80680003);
const int RESULT_FRIENDS_SERVER_ERROR_260 = static_cast<int>(0x80680004);
const int RESULT_FRIENDS_SERVER_ERROR_261 = static_cast<int>(0x80680005);
const int RESULT_NO_PASSWORD = static_cast<int>(0x80030065);
// the friends server is busy with another request
const u32 RESULT_FRIENDS_BUSY = 0xC8A0C7F0;
// the successful results of the friends server
const s32 HTTP_STATUS_OK = 200;
const s32 GAME_AUTHENTICATION_SUCCESS = 1;
const s32 SERVICE_LOCATOR_SUCCESS = 7;
// the descriptions of failed friends requests with their own results
const u32 DESCRIPTION_260 = 260;
const u32 DESCRIPTION_261 = 261;
// the description of a wait that timed out
const u32 DESCRIPTION_TIMEOUT = 0x3FE;
// the service of the service locator request
const char* const SERVICE_LOCATOR_PREFIX = "0000";

// a waiting request: completed when the timeout passed, else checked again later
inline void WaitOrTimeOut(JobLogin* pJob)
{
    if (pJob->IsTimedOut()) {
        pJob->Complete(qResult(RESULT_TIMEOUT));
    } else {
        pJob->SetToWaiting(JobLogin::WAIT_TIME);
    }
}

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

// 0x003586D4 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
// 0x00358668 (deleting dtor)
nn::nex::JobCTRLogin::~JobCTRLogin()
{
    if (m_IsEventInitialized) {
        m_Event.Close();
        m_IsEventInitialized = false;
    }
}

// 0x0035857C slot 0x34 | fefates:bytes
void nn::nex::JobCTRLogin::StepFirst()
{
    nnfriesndsGameAuthenticationData data;
    nn::Result result = nn::friends::CTR::detail::GetGameAuthenticationData(&data);
    if (result.IsFailure() || data.httpStatusCode != HTTP_STATUS_OK || data.result != GAME_AUTHENTICATION_SUCCESS) {
        SetStep(Step(&JobCTRLogin::StepGetGameAuthentication, ""));
    } else if (m_pKeyHash != nullptr) {
        SetStep(Step(&JobCTRLogin::StepRequestAuthenticationToken, ""));
    } else {
        SetStep(Step(&JobCTRLogin::StepLogin, ""));
    }
}

// 0x00357C40 | fefates:bytes [tier B]
void nn::nex::JobCTRLogin::InitializeEvent()
{
    if (m_IsEventInitialized) {
        m_Event.Close();
    }
    m_Event.Initialize(nn::os::RESET_TYPE_ONESHOT);
    m_Event.ClearSignal();
    m_IsEventInitialized = true;
}

// 0x00357920 (name is ours)
void nn::nex::JobCTRLogin::StepLogin()
{
    u32 principalId = nn::friends::CTR::detail::GetMyPrincipalId();
    nnfriesndsGameAuthenticationData data;
    char password[0x20] = {};
    nn::friends::CTR::detail::GetGameAuthenticationData(&data);
    m_pBackEndServices->m_LastHttpStatusCode = data.httpStatusCode;
    m_pBackEndServices->m_LastResponseResult = data.result;
    if (data.httpStatusCode != HTTP_STATUS_OK || data.result != GAME_AUTHENTICATION_SUCCESS) {
        Complete(qResult(RESULT_AUTHENTICATION_REJECTED));
        return;
    }
    if (nn::friends::CTR::detail::GetMyPassword(password).IsFailure()) {
        Complete(qResult(RESULT_NO_PASSWORD));
        return;
    }
    if (m_Timeout == -1) {
        m_pBackEndServices->m_LoginTimeout = -1;
    } else {
        s32 elapsed = static_cast<s32>(Time::GetTime().m_Value - m_StartTime.m_Value);
        if (m_Timeout <= elapsed) {
            Complete(qResult(RESULT_TIMEOUT));
            return;
        }
        m_pBackEndServices->m_LoginTimeout = m_Timeout - elapsed;
    }
    RendezVous* pRendezVous = static_cast<RendezVous*>(m_pBackEndServices);
    m_CallContext.Reset();
    m_CustomData.Reset(new AuthenticationInfo());
    StringStream userName;
    String host(data.serverAddress);
    static_cast<AuthenticationInfo*>(m_CustomData.m_pObject)->m_Token = data.token;
    userName.StreamNumber(principalId);
    if (m_pBackEndServices->m_Unknown0x140) {
        static_cast<AuthenticationInfo*>(m_CustomData.m_pObject)->m_ServerVersion = static_cast<u32>(-1);
    }
    if (!pRendezVous->Login(&m_CallContext, userName.CStr(), password, host.m_pString, data.serverPort, m_CustomData, &m_pCredentials, &m_PrincipalName)) {
        Complete(qResult(RESULT_CALL_FAILED));
        return;
    }
    SetStep(Step(&JobCTRLogin::StepWaitingForGameLogin, ""));
}

// 0x00357CDC | fefates:bytes [tier B]
void nn::nex::JobCTRLogin::StepWaitingForGameLogin()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    u8 state = m_CallContext.m_State;
    if (state != CallContext::STATE_SUCCESS && state != CallContext::STATE_FAILURE && state != CallContext::STATE_CANCELLED) {
        SetToWaiting(WAIT_TIME);
        return;
    }
    qResult result = m_CallContext.m_Result;
    if (result == qResult(RESULT_AUTHENTICATION_EXPIRED)) {
        SetStep(Step(&JobCTRLogin::StepGetGameAuthentication, ""));
        return;
    }
    Complete(result);
}

// 0x00357DBC | fefates:bytes [tier B]
void nn::nex::JobCTRLogin::StepGetGameAuthentication()
{
    wchar_t name[12] = {};
    InitializeEvent();
    nn::Result result = nn::friends::CTR::detail::RequestGameAuthenticationWithoutLogin(&m_Event, m_Unknown0xE4, name, 0, 0);
    if (result.IsSuccess()) {
        SetStep(Step(&JobCTRLogin::StepWaitingForGameAuthentication, ""));
        return;
    }
    if (result.GetPrintableBits() != RESULT_FRIENDS_BUSY) {
        Complete(qResult(RESULT_REQUEST_FAILED));
        return;
    }
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        Complete(qResult(RESULT_TIMEOUT));
        return;
    }
    WaitOrTimeOut(this);
}

// 0x00357F2C | fefates:bytes [tier B]
void nn::nex::JobCTRLogin::StepRequestAuthenticationToken()
{
    char keyHash[12] = {};
    char service[8];
    std::memcpy(service, SERVICE_LOCATOR_PREFIX, sizeof(service));
    InitializeEvent();
    std::strncpy(keyHash, m_pKeyHash, 8);
    nn::Result result = nn::friends::CTR::detail::RequestServiceLocatorWithoutLogin(&m_Event, m_Unknown0xE4, keyHash, service, 0, 0);
    if (result.IsSuccess()) {
        SetStep(Step(&JobCTRLogin::StepWaitingForRequestAuthenticationToken, ""));
        return;
    }
    if (result.GetPrintableBits() != RESULT_FRIENDS_BUSY) {
        Complete(qResult(RESULT_REQUEST_FAILED));
        return;
    }
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        Complete(qResult(RESULT_TIMEOUT));
        return;
    }
    WaitOrTimeOut(this);
}

// 0x003580B8 (name is ours)
void nn::nex::JobCTRLogin::StepWaitingForGameAuthentication()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        Complete(qResult(RESULT_TIMEOUT));
        return;
    }
    nn::Result waitResult = nn::svc::WaitSynchronization1(m_Event.GetHandle(), 0);
    if (waitResult.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(waitResult);
    }
    if (waitResult.GetDescription() == DESCRIPTION_TIMEOUT) {
        WaitOrTimeOut(this);
        return;
    }
    nn::Result result = nn::friends::CTR::detail::GetLastResponseResult();
    if (result.IsFailure()) {
        Complete(qResult(GetFriendsErrorResult(result)));
        return;
    }
    nnfriesndsGameAuthenticationData data;
    nn::friends::CTR::detail::GetGameAuthenticationData(&data);
    m_pBackEndServices->m_LastHttpStatusCode = data.httpStatusCode;
    m_pBackEndServices->m_LastResponseResult = data.result;
    if (data.httpStatusCode != HTTP_STATUS_OK || data.result != GAME_AUTHENTICATION_SUCCESS) {
        Complete(qResult(RESULT_AUTHENTICATION_REJECTED));
        return;
    }
    if (m_pKeyHash != nullptr) {
        SetStep(Step(&JobCTRLogin::StepRequestAuthenticationToken, ""));
    } else {
        SetStep(Step(&JobCTRLogin::StepLogin, ""));
    }
}

// 0x00358324 | fefates:bytes [tier B]
void nn::nex::JobCTRLogin::StepWaitingForRequestAuthenticationToken()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        SetToComplete();
        return;
    }
    if (Core::GetInstance()->m_IsInterrupted) {
        Complete(qResult(RESULT_TIMEOUT));
        return;
    }
    nn::Result waitResult = nn::svc::WaitSynchronization1(m_Event.GetHandle(), 0);
    if (waitResult.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(waitResult);
    }
    if (waitResult.GetDescription() == DESCRIPTION_TIMEOUT) {
        WaitOrTimeOut(this);
        return;
    }
    nn::Result result = nn::friends::CTR::detail::GetLastResponseResult();
    if (result.IsFailure()) {
        Complete(qResult(GetFriendsErrorResult(result)));
        return;
    }
    nn::friends::CTR::detail::GetServiceLocatorData(&m_ServiceLocatorData);
    m_pBackEndServices->m_LastHttpStatusCode = m_ServiceLocatorData.httpStatusCode;
    m_pBackEndServices->m_LastResponseResult = m_ServiceLocatorData.result;
    if (m_ServiceLocatorData.httpStatusCode != HTTP_STATUS_OK || m_ServiceLocatorData.result != SERVICE_LOCATOR_SUCCESS) {
        m_pBackEndServices->m_LastResponseResult = m_ServiceLocatorData.result + 4000;
        Complete(qResult(RESULT_AUTHENTICATION_REJECTED));
        return;
    }
    std::memcpy(m_pServiceToken, m_ServiceLocatorData.serviceToken, sizeof(m_ServiceLocatorData.serviceToken));
    m_pServiceToken[sizeof(m_ServiceLocatorData.serviceToken) - 1] = '\0';
    SetStep(Step(&JobCTRLogin::StepLogin, ""));
}

} // namespace nex
} // namespace nn
