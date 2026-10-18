#include "nn/nex/nex_JobTicketManagerLogin.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_TicketGrantingProtocolClient.h"
#include "nn/nex/nex_TicketManager.h"

#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results of the login (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_INVALID_KEY = static_cast<int>(0x80030065);
const int RESULT_INVALID_TICKET = static_cast<int>(0x8003006A);
const int RESULT_NOT_AUTHENTICATED = static_cast<int>(0x8001000B);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_KEY_FAILED = static_cast<int>(0x80010001);
} // namespace

// 0x00399DA4 | fefates:callgraph [tier C]
void nn::nex::JobTicketManagerLogin::CompleteJob(nn::nex::CallContext::State state, nn::nex::qResult result)
{
    SetToComplete();
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(state, result, true);
    }
}

// 0x00399DF4 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLogin::ValidateKey()
{
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS && !Core::s_IsSchedulerThreaded) {
        CompleteJob(static_cast<CallContext::State>(m_CallContext.m_State), m_CallContext.m_Result);
        return;
    }
    KerberosEncryption* pEncryption = m_pManager->m_pClient->m_pKerberos;
    pEncryption->SetKey(m_Key);
    Buffer ticket;
    if (m_pManager->m_pClient->m_pKerberos->Decrypt(m_Ticket, &ticket)) {
        *m_pResult = qResult(RESULT_SUCCESS);
        CompleteJob(static_cast<CallContext::State>(m_ProtocolCallContext.m_State), qResult(RESULT_SUCCESS));
    } else {
        *m_pResult = qResult(RESULT_INVALID_KEY);
        CompleteJob(CallContext::STATE_SUCCESS, qResult(RESULT_INVALID_TICKET));
    }
}

// 0x00399F6C | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLogin::InitiateLogin()
{
    m_ProtocolCallContext.m_Deadline = m_pManager->m_pClient->m_CallDeadline;
    bool isCalled;
    if (m_pData != nullptr) {
        isCalled = m_pManager->m_pClient->m_pProtocol->CallLoginEx(&m_ProtocolCallContext, m_pResult, m_Name, m_pPrincipalId, &m_Ticket, m_pConnectionData,
                                                                  m_pServerName, *m_pData);
    } else {
        isCalled = m_pManager->m_pClient->m_pProtocol->CallLogin(&m_ProtocolCallContext, m_pResult, m_Name, m_pPrincipalId, &m_Ticket, m_pConnectionData,
                                                                m_pServerName);
    }
    if (!isCalled) {
        CompleteJob(CallContext::STATE_FAILURE, qResult(RESULT_CALL_FAILED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_CALL_IN_PROGRESS) {
        SetStep(Step(&JobTicketManagerLogin::ProcessLoginResult, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobTicketManagerLogin::ProcessLoginResult, ""));
}

// 0x0039A0F0 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLogin::ProcessLoginResult()
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
    *m_pResult = qResult(RESULT_NOT_AUTHENTICATED);
    SetStep(Step(&JobTicketManagerLogin::DeriveKey, ""));
}

// 0x0039A1D4 | fefates:bytes [tier B]
void nn::nex::JobTicketManagerLogin::DeriveKey()
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
        SetStep(Step(&JobTicketManagerLogin::ValidateKey, ""));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobTicketManagerLogin::ValidateKey, ""));
}

// 0x0039A320 | mk7dlp:callseq [tier A]
nn::nex::JobTicketManagerLogin::JobTicketManagerLogin(unsigned callContextId, nn::nex::TicketManager* pManager, nn::nex::qResult* pResult, const nn::nex::String& name,
                                                      const char* pPassword, unsigned* pPrincipalId, nn::nex::RVConnectionData* pConnectionData, nn::nex::String* pServerName,
                                                      nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData)
    : StepSequenceJob(static_cast<JobType>(2), DebugString())
{
    m_CallContextId = callContextId;
    m_pManager = pManager;
    m_Name = name;
    m_pPassword = NewArray<char>(strlen(pPassword) + 1);
    strcpy(m_pPassword, pPassword);
    m_pPrincipalId = pPrincipalId;
    m_pResult = pResult;
    m_pServerName = pServerName;
    m_pConnectionData = pConnectionData;
    m_pData = pData;
    SetStep(Step(&JobTicketManagerLogin::InitiateLogin, ""));
}

// 0x0039A4A4 slot 0x00
// 0x0039A448 (deleting dtor)
nn::nex::JobTicketManagerLogin::~JobTicketManagerLogin()
{
    if (m_pPassword != nullptr) {
        MemoryManager::Free(reinterpret_cast<u32*>(m_pPassword) - 1);
    }
}

} // namespace nex
} // namespace nn
