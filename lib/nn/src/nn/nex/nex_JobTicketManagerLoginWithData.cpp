#include "nn/nex/nex_JobTicketManagerLoginWithData.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_TicketGrantingProtocolClient.h"
#include "nn/nex/nex_TicketManager.h"

namespace nn {
namespace nex {
namespace {
// the results of the login (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_INVALID_KEY = static_cast<int>(0x80030065);
const int RESULT_INVALID_TICKET = static_cast<int>(0x8003006A);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_KEY_FAILED = static_cast<int>(0x80010001);
} // namespace

// 0x003C097C | fefates:callgraph [tier C]
void nn::nex::JobTicketManagerLoginWithData::CompleteJob(nn::nex::CallContext::State state, nn::nex::qResult result)
{
    SetToComplete();
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(state, result, true);
    }
}

// 0x003C09CC | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLoginWithData::ValidateKey()
{
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS && !Core::s_IsSchedulerThreaded) {
        CompleteJob(static_cast<CallContext::State>(m_CallContext.m_State), m_CallContext.m_Result);
        return;
    }
    KerberosEncryption* pEncryption = m_pManager->m_pClient->m_pKerberos;
    pEncryption->SetKey(m_Key);
    Buffer ticket;
    if (m_pManager->m_pClient->m_pKerberos->Decrypt(m_Ticket, &ticket)) {
        CompleteJob(static_cast<CallContext::State>(m_ProtocolCallContext.m_State), qResult(RESULT_SUCCESS));
    } else {
        *m_pResult = qResult(RESULT_INVALID_KEY);
        CompleteJob(CallContext::STATE_SUCCESS, qResult(RESULT_INVALID_TICKET));
    }
}

// 0x003C0B24 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLoginWithData::InitiateLogin()
{
    m_ProtocolCallContext.m_Deadline = m_pManager->m_pClient->m_CallDeadline;
    if (!m_pManager->m_pClient->m_pProtocol->CallLoginWithContext(&m_ProtocolCallContext, m_pResult, *m_pData, m_pPrincipalId, &m_Ticket, m_pConnectionData)) {
        CompleteJob(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_CALL_IN_PROGRESS) {
        SetStep(Step(&JobTicketManagerLoginWithData::ProcessLoginResult, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobTicketManagerLoginWithData::ProcessLoginResult, ""));
}

// 0x003C0C54 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLoginWithData::ProcessLoginResult()
{
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(CallContext::STATE_FAILURE, m_ProtocolCallContext.m_Result);
        return;
    }
    bool isSuccess = true;
    if (!m_pResult->Equals(isSuccess)) {
        CompleteJob(static_cast<CallContext::State>(m_ProtocolCallContext.m_State), m_ProtocolCallContext.m_Result);
        return;
    }
    SetStep(Step(&JobTicketManagerLoginWithData::DeriveKey, ""));
}

// 0x003C0D14 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLoginWithData::DeriveKey()
{
    if (Core::s_IsSchedulerThreaded) {
        m_Key = KerberosEncryption::CreateKey(*m_pPrincipalId, m_pPassword);
    } else {
        m_CallContext.m_Deadline = m_pManager->m_pClient->m_CallDeadline;
        if (!KerberosEncryption::CreateKey(&m_CallContext, *m_pPrincipalId, m_pPassword, &m_Key)) {
            CompleteJob(CallContext::STATE_FAILURE, qResult(RESULT_KEY_FAILED));
            return;
        }
    }
    if (m_CallContext.m_State != CallContext::STATE_CALL_IN_PROGRESS) {
        SetStep(Step(&JobTicketManagerLoginWithData::ValidateKey, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobTicketManagerLoginWithData::ValidateKey, ""));
}

// 0x003C0EBC slot 0x00
// 0x003C0E68 (deleting dtor)
nn::nex::JobTicketManagerLoginWithData::~JobTicketManagerLoginWithData()
{
    if (m_pPassword != nullptr) {
        MemoryManager::Free(reinterpret_cast<u32*>(m_pPassword) - 1);
    }
}

} // namespace nex
} // namespace nn
