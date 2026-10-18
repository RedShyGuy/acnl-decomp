#include "nn/nex/nex_JobCheckAndRelay.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the time of the check of the public port (ms)
const int PUBLIC_PORT_TIMEOUT = 5000;
// the time between the tests of a waiting step (ms)
const int WAIT_TIME = 50;

// (inline) the root transport of the network (null if there is none)
inline RootTransport* GetRootTransport()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}
} // namespace

// 0x0037F2D4 (name is ours)
void nn::nex::JobCheckAndRelay::Fail()
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(qResult::FAILURE), false);
    }
    SetState(STATE_COMPLETED);
}

// 0x0037F350 (name is ours)
void nn::nex::JobCheckAndRelay::WaitForPublicPort()
{
    if (GetRootTransport() == nullptr || Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr ||
        m_Deadline < SystemClock::GetTime()) {
        SetStepInline(&JobCheckAndRelay::Fail, "");
        return;
    }
    if (m_Context.m_State == CallContext::STATE_SUCCESS) {
        NATProperties properties = GetRootTransport()->GetNATProperties();
        SetStepInline(&JobCheckAndRelay::SendProbeRequest, "");
        return;
    }
    if (m_Context.m_State == CallContext::STATE_FAILURE) {
        SetStepInline(&JobCheckAndRelay::Fail, "");
    }
    SetState(STATE_WAITING);
    m_WaitTime = WAIT_TIME;
}

// 0x0037F4C4 (name is ours)
void nn::nex::JobCheckAndRelay::WaitForProbes()
{
    if (Network::GetInstance() == nullptr) {
        SetStepInline(&JobCheckAndRelay::Fail, "");
        return;
    }
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext == nullptr) {
        SetStepInline(&JobCheckAndRelay::Fail, "");
        return;
    }
    if (m_Deadline < SystemClock::GetTime()) {
        SetStepInline(&JobCheckAndRelay::Fail, "");
        return;
    }
    NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
    bool isFailed = true;
    bool isSucceeded = false;
    for (qList<StationURL>::Node* pNode = m_URLs.GetFirst(); !m_URLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        u32 rvcid = pNode->m_Value.GetParamValue(L"RVCID", 0);
        URLProbe* pProbe = nullptr;
        s32 result = -1;
        {
            ScopedCS lock(GetSchedulerLock());
            for (qList<URLProbe>::Node* pProbeNode = pEngine->m_FinishedProbes.GetFirst(); !pEngine->m_FinishedProbes.IsEnd(pProbeNode); pProbeNode = pProbeNode->m_pNext) {
                if (pProbeNode->m_Value.m_URL.GetParamValue(L"RVCID", 0) == rvcid) {
                    pProbe = &pProbeNode->m_Value;
                    break;
                }
            }
            if (pProbe != nullptr) {
                result = pProbe->m_Result;
            }
        }
        if (pProbe != nullptr && result != -1) {
            isSucceeded = true;
        }
        if (!pEngine->IsProbeFailed(pNode->m_Value)) {
            isFailed = false;
        }
    }
    if (isSucceeded) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(qResult::SUCCESS), true);
        SetState(STATE_COMPLETED);
        return;
    }
    if (isFailed) {
        SetStepInline(&JobCheckAndRelay::Fail, "");
        return;
    }
    SetState(STATE_WAITING);
    m_WaitTime = WAIT_TIME;
}

// 0x0037F768 (name is ours)
void nn::nex::JobCheckAndRelay::CheckPublicPort()
{
    if (Network::GetInstance() != nullptr && Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) != nullptr) {
        NATRelayInterface* pInterface = Network::GetInstance()->m_pNATTraversalEngine->m_pRelayInterface;
        if (pInterface != nullptr) {
            InetAddress address;
            pInterface->CheckCurrentPublicPort(&m_Context, address, true);
            Core::GetInstance()->m_pCallContextRegister->m_Associations.insert(std::pair<const u32, u32>(m_CallContextId, m_Context.m_Id));
            SetStepInline(&JobCheckAndRelay::WaitForPublicPort, "");
            m_Deadline = SystemClock::GetTime() + PUBLIC_PORT_TIMEOUT;
            return;
        }
    }
    SetStepInline(&JobCheckAndRelay::Fail, "");
}

// 0x0037F8A8 (name is ours)
void nn::nex::JobCheckAndRelay::SendProbeRequest()
{
    if (Network::GetInstance() != nullptr && GetRootTransport() != nullptr &&
        Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) != nullptr) {
        NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
        NATRelayInterface* pInterface = pEngine->m_pRelayInterface;
        if (pInterface != nullptr) {
            NATProperties properties = GetRootTransport()->GetNATProperties();
            // the NAT maps the next destination to the next port
            m_URL.SetPortNumberInline(static_cast<u16>(properties.m_PublicPort + properties.m_PortIncrement));
            pInterface->RelayProbeRequest(m_URLs, m_URL);
            if (m_URL.GetParamValue(L"probeinit", 0) == 0) {
                pEngine->StartSendProbe(m_URLs.front());
            }
            SetStepInline(&JobCheckAndRelay::WaitForProbes, "");
            m_Deadline = SystemClock::GetTime() + Network::GetInstance()->m_pRootTransport->GetCheckAndRelayTimeout();
            return;
        }
    }
    SetStepInline(&JobCheckAndRelay::Fail, "");
}

// 0x0037FAD4 slot 0x00
// 0x0037FA80 (deleting dtor)
nn::nex::JobCheckAndRelay::~JobCheckAndRelay()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
