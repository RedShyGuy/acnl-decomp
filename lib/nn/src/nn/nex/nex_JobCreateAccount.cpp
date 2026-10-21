#include "nn/nex/nex_JobCreateAccount.h"
#include "nn/nex/nex_AccountManagementClient.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobManageAccount.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the result of a created account that already exists
const int RESULT_ACCOUNT_EXISTS = 0x10001;

// (inline) a job of the account management call is queued; true (also if the call context is
// busy)
inline bool StartManageAccountJob(ProtocolCallContext* pContext, AccountManagementProtocolClient* pProtocol, const String& principalName, const String& password, u32 groups,
                                  const String& email, qResult* pResult, JobManageAccount::FuncType funcType, JobManageAccount::DataHolder* pData1,
                                  JobManageAccount::DataHolder* pData2, JobManageAccount::DataHolder* pData3, unsigned int* pPid, String* pPidHMAC)
{
    Core* pCore = Core::GetInstance();
    ScopedCS lock((pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock);
    if (pContext->InitiateCall()) {
        JobManageAccount* pJob = new JobManageAccount(pContext->m_Id, pProtocol, principalName, password, groups, email, pResult, funcType, pData1, pData2, pData3, pPid,
                                                      pPidHMAC);
        Core* pQueueCore = Core::GetInstance();
        (pQueueCore != nullptr ? pQueueCore->m_pScheduler : nullptr)->Queue(pJob, false);
    }
    return true;
}
} // namespace

// 0x0037FB10 | fefates:bytes [tier B]
void nn::nex::JobCreateAccount::LoginGuest()
{
    if (!m_pBackEndServices->LoginImpl(&m_LoginContext, nullptr, L"guest", m_pGuestPassword, m_pAddress, m_Port, &m_pGuestCredentials, nullptr, nullptr)) {
        CompleteJob(m_LoginContext.m_Result);
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_LoginContext, new Step(&JobCreateAccount::ProcessLoginGuestResult, ""));
}

// 0x0037FBEC | fefates:bytes [tier B]
void nn::nex::JobCreateAccount::CompleteJob(nn::nex::qResult result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result && m_Result) {
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result ? m_Result : result);
        }
    }
    SetToComplete();
}

// 0x0037FC84 | fefates:bytes [tier B]
void nn::nex::JobCreateAccount::LogoutGuest()
{
    if (!m_pBackEndServices->Logout_(&m_LogoutContext, m_pGuestCredentials)) {
        CompleteJob(m_LogoutContext.m_Result);
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_LogoutContext, new Step(&JobCreateAccount::ProcessLogoutGuestResult, ""));
}

// 0x0037FD28 (name is ours)
void nn::nex::JobCreateAccount::CallCreateAccount()
{
    m_pClient->Bind(m_pGuestCredentials);
    if (!StartManageAccountJob(&m_ManageAccountContext, m_pClient->GetProtocol(), m_PrincipalName, String(m_pPassword), m_Groups, m_Email, &m_CreateAccountResult,
                               JobManageAccount::FUNC_CREATE_ACCOUNT, nullptr, nullptr, nullptr, nullptr, nullptr)) {
        m_Result = m_ManageAccountContext.m_Result;
        SetStep(Step(&JobCreateAccount::LogoutGuest, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ManageAccountContext, new Step(&JobCreateAccount::ProcessCreateAccountResult, ""));
}

// 0x0037FF14 (name is ours)
void nn::nex::JobCreateAccount::CallCustomCreateAccount()
{
    m_pClient->Bind(m_pGuestCredentials);
    if (!StartManageAccountJob(&m_ManageAccountContext, m_pClient->GetProtocol(), m_PrincipalName, String(m_pPassword), m_Groups, m_Email, nullptr,
                               JobManageAccount::FUNC_CUSTOM_CREATE_ACCOUNT, nullptr, nullptr, &m_AuthData, m_pPid, nullptr)) {
        m_Result = m_ManageAccountContext.m_Result;
        SetStep(Step(&JobCreateAccount::LogoutGuest, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ManageAccountContext, new Step(&JobCreateAccount::ProcessCreateAccountResult, ""));
}

// 0x00380100 (name is ours)
void nn::nex::JobCreateAccount::CallChangePasswordByGuest()
{
    m_pClient->Bind(m_pGuestCredentials);
    if (!StartManageAccountJob(&m_ManageAccountContext, m_pClient->GetProtocol(), m_PrincipalName, String(m_pPassword), 0, m_Email, nullptr,
                               JobManageAccount::FUNC_CHANGE_PASSWORD_BY_GUEST, nullptr, nullptr, nullptr, nullptr, nullptr)) {
        m_Result = m_ManageAccountContext.m_Result;
        SetStep(Step(&JobCreateAccount::LogoutGuest, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ManageAccountContext, new Step(&JobCreateAccount::ProcessCreateAccountResult, ""));
}

// 0x003802DC (name is ours)
void nn::nex::JobCreateAccount::CallNintendoCreateAccount()
{
    m_pClient->Bind(m_pGuestCredentials);
    if (!StartManageAccountJob(&m_ManageAccountContext, m_pClient->GetProtocol(), m_PrincipalName, String(m_pPassword), m_Groups, m_Email, nullptr,
                               JobManageAccount::FUNC_NINTENDO_CREATE_ACCOUNT, nullptr, nullptr, &m_AuthData, m_pPid, m_pPidHMAC)) {
        m_Result = m_ManageAccountContext.m_Result;
        SetStep(Step(&JobCreateAccount::LogoutGuest, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ManageAccountContext, new Step(&JobCreateAccount::ProcessCreateAccountResult, ""));
}

// 0x003804CC (name is ours)
void nn::nex::JobCreateAccount::ProcessLoginGuestResult()
{
    if (m_LoginContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_LoginContext.m_Result);
        return;
    }
    switch (m_FuncType) {
    case FUNC_CREATE_ACCOUNT:
        SetStep(Step(&JobCreateAccount::CallCreateAccount, ""));
        break;
    case FUNC_CUSTOM_CREATE_ACCOUNT:
        SetStep(Step(&JobCreateAccount::CallCustomCreateAccount, ""));
        break;
    case FUNC_CHANGE_PASSWORD_BY_GUEST:
        SetStep(Step(&JobCreateAccount::CallChangePasswordByGuest, ""));
        break;
    case FUNC_NINTENDO_CREATE_ACCOUNT:
        SetStep(Step(&JobCreateAccount::CallNintendoCreateAccount, ""));
        break;
    case FUNC_CREATE_ACCOUNT_WITH_CUSTOM_DATA:
        SetStep(Step(&JobCreateAccount::CallCreateAccountWithCustomData, ""));
        break;
    }
}

// 0x00380610 | fefates:bytes [tier B]
void nn::nex::JobCreateAccount::ProcessLogoutGuestResult()
{
    m_pGuestCredentials = nullptr;
    CompleteJob(m_LogoutContext.m_Result);
}

// 0x0038063C | fefates:bytes [tier B]
void nn::nex::JobCreateAccount::ProcessCreateAccountResult()
{
    if (m_ManageAccountContext.m_State != CallContext::STATE_SUCCESS) {
        m_Result = m_ManageAccountContext.m_Result;
    } else if (m_FuncType == FUNC_CREATE_ACCOUNT) {
        if (m_CreateAccountResult) {
            m_Result = qResult(RESULT_ACCOUNT_EXISTS);
        } else {
            m_Result = m_CreateAccountResult;
        }
    }
    if (m_pClient != nullptr) {
        delete m_pClient;
    }
    m_pClient = nullptr;
    SetStep(Step(&JobCreateAccount::LogoutGuest, ""));
}

// 0x00380734 (name is ours)
void nn::nex::JobCreateAccount::CallCreateAccountWithCustomData()
{
    m_pClient->Bind(m_pGuestCredentials);
    if (!StartManageAccountJob(&m_ManageAccountContext, m_pClient->GetProtocol(), m_PrincipalName, String(m_pPassword), m_Groups, m_Email, nullptr,
                               JobManageAccount::FUNC_CREATE_ACCOUNT_WITH_CUSTOM_DATA, &m_PublicData, &m_PrivateData, nullptr, nullptr, nullptr)) {
        m_Result = m_ManageAccountContext.m_Result;
        SetStep(Step(&JobCreateAccount::LogoutGuest, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ManageAccountContext, new Step(&JobCreateAccount::ProcessCreateAccountResult, ""));
}

} // namespace nex
} // namespace nn
