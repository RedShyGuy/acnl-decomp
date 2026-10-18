#include "nn/nex/nex_JobStartNATSession.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobCallContextCallback.h"
#include "nn/nex/nex_JobNameResolve.h"
#include "nn/nex/nex_JobResolveRelayInfo.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the results of the job (the names are ours)
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_NAME_RESOLUTION_FAILED = static_cast<int>(0x80050014);
// the time between the tests of a waiting step (ms)
const int WAIT_TIME = 10;

// (inline) the root transport of the network (null if there is none)
inline RootTransport* GetRootTransport()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}

// (inline) the call of the caller (null if it was cancelled)
inline CallContext* GetCallContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// 0x0038B990 | fefates:bytes [tier B]
void nn::nex::JobStartNATSession::StepFinish()
{
    if (m_pRelayInfo != nullptr) {
        qResult result = m_RelayInfoContext.m_Result;
        if (result) {
            m_pRelayInfo->ResolvedRelayInfo();
        }
    }
    CompleteJob(qResult(m_SessionContext.m_Result));
}

// 0x0038B9E4 | fefates:bytes [tier B]
void nn::nex::JobStartNATSession::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(result.m_Code >= 0 ? CallContext::STATE_SUCCESS : CallContext::STATE_FAILURE, result, true);
    } else {
        m_SessionContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetState(STATE_COMPLETED);
}

// 0x0038BA58 (name is ours)
void nn::nex::JobStartNATSession::ResolveRelayServer()
{
    if (Network::GetInstance() == nullptr || GetRootTransport() == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_RelayInfoContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetState(STATE_SUSPENDED);
        ResumeOnCallCompletion(&m_RelayInfoContext, new Step(&JobStartNATSession::ResolveRelayServer, ""));
        return;
    }
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_pRelayInfo == nullptr || static_cast<u8>(m_pRelayInfo->m_AddressType) != JobResolveRelayInfo::ADDRESS_TYPE_RELAY_ADDRESS ||
        GetRootTransport()->m_IsRelayAddressResolved) {
        SetStepInline(&JobStartNATSession::StepFinish, "");
        return;
    }
    GetRootTransport()->m_RelayServerName = m_pRelayInfo->m_Address;
    m_NameResolveContext.Reset();
    if (!m_NameResolveContext.InitiateCall()) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    JobNameResolve* pJob = new JobNameResolve(g_NameResolutionThreadPriority, g_NameResolutionCoreNo, &RootTransport::ResolveRelayAddress);
    pJob->Initialize(m_NameResolveContext.m_Id);
    Scheduler::GetInstanceInline()->Queue(pJob, true);
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_NameResolveContext.m_Id);
    SetState(STATE_SUSPENDED);
    ResumeOnCallCompletion(&m_NameResolveContext, new Step(&JobStartNATSession::WaitForRelayServer, ""));
}

// 0x0038BDE8 (name is ours)
void nn::nex::JobStartNATSession::WaitForRelayServer()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        // (the step table of the original has the step of JobCallContextCallback here)
        SetStepInline(Step(&JobCallContextCallback::ProcessCallResult, ""));
        return;
    }
    switch (m_NameResolveContext.m_State) {
    case CallContext::STATE_CALL_IN_PROGRESS:
        SetToWaiting(WAIT_TIME);
        return;
    case CallContext::STATE_SUCCESS:
        SetStepInline(&JobStartNATSession::StepFinish, "");
        return;
    case CallContext::STATE_FAILURE:
        CompleteJob(qResult(RESULT_NAME_RESOLUTION_FAILED));
        return;
    default:
        SetStepInline(&JobStartNATSession::StepFinish, "");
        return;
    }
}

// 0x0038BEF8 (name is ours)
void nn::nex::JobStartNATSession::WaitForSession()
{
    if (GetCallContext(m_CallContextId) != nullptr && m_SessionContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        SetState(STATE_SUSPENDED);
        ResumeOnCallCompletion(&m_SessionContext, new Step(&JobStartNATSession::ResolveRelayServer, ""));
        return;
    }
    SetStepInline(&JobStartNATSession::ResolveRelayServer, "");
}

} // namespace nex
} // namespace nn
