#include "nn/nex/nex_JobLoginOrCreateAccount.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_Time.h"

namespace nn {
namespace nex {
namespace {
// the call could not be sent
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
// the time of the job is over
const int RESULT_TIMEOUT = static_cast<int>(0x8001000B);
// the account of the login does not exist
const int RESULT_INVALID_USERNAME = static_cast<int>(0x80030064);
} // namespace

// 0x003B0D74 | fefates:bytes [tier B]
void nn::nex::JobLoginOrCreateAccount::CreateAccount()
{
    if (!m_pCreateAccount->Call(&m_CreateAccountContext)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_CreateAccountContext, new Step(&JobLoginOrCreateAccount::ProcessCreateAccountResult, ""));
}

// 0x003B0E28 | fefates:bytes [tier B]
void nn::nex::JobLoginOrCreateAccount::ProcessRVLoginResult()
{
    if (m_LoginContext.m_State == CallContext::STATE_SUCCESS) {
        CompleteJob(m_LoginContext.m_Result);
        return;
    }
    if (m_LoginContext.m_Result.m_Code != qResult(RESULT_INVALID_USERNAME).m_Code || m_IsAccountCreated) {
        CompleteJob(m_LoginContext.m_Result);
        return;
    }
    m_IsAccountCreated = true;
    SetStep(Step(&JobLoginOrCreateAccount::CreateAccount, ""));
}

// 0x003B0F0C | fefates:bytes [tier B]
void nn::nex::JobLoginOrCreateAccount::ProcessCreateAccountResult()
{
    if (m_CreateAccountContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_CreateAccountContext.m_Result);
        return;
    }
    m_LoginContext.Reset();
    SetStep(Step(&JobLoginOrCreateAccount::LoginRV, ""));
}

// 0x003B0F90 (name is ours)
void nn::nex::JobLoginOrCreateAccount::LoginRV()
{
    if (m_pBackEndServices->m_LoginJobTimeout == 0) {
        s32 remaining = static_cast<s32>(m_Deadline - static_cast<u32>(Time::GetTime().m_Value));
        if (remaining < 0) {
            CompleteJob(qResult(RESULT_TIMEOUT));
            return;
        }
        m_pBackEndServices->m_LoginJobTimeout = remaining;
    }
    char* pPassword = nullptr;
    m_Password.CreateCopy(&pPassword);
    if (!m_pBackEndServices->LoginImpl(&m_LoginContext, nullptr, m_pName, pPassword, m_pAddress, m_Port, m_ppCredentials, m_pServerName, m_pData)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
    } else {
        SetToSuspended();
        ResumeOnCallCompletion(&m_LoginContext, new Step(&JobLoginOrCreateAccount::ProcessRVLoginResult, ""));
    }
    String::ReleaseCopy(pPassword);
}

} // namespace nex
} // namespace nn
