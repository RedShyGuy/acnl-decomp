#include "nn/nex/nex_JobConnectEndPoint.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_NATRelayInterface.h"
#include "nn/nex/nex_NATTraversalEngine.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Router.h"
#include "nn/nex/nex_RoutingStream.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_TransportAdapter.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
// a URL with a relay always gets a NAT traversal (name is ours)
// 0x0096C931
bool g_UsesNATTraversalForRelay = true;

namespace {
// the connection failed
const int RESULT_CONNECTION_FAILED = static_cast<int>(0x80050002);
// the connection failed with fault error 8 of the end point
const int RESULT_CONNECTION_REFUSED = static_cast<int>(0x8068000B);
// the call was cancelled
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
// the time between the tests of a waiting step (ms)
const int WAIT_TIME = 100;
// the fault error of the end point that fails the job with RESULT_CONNECTION_REFUSED
const u32 FAULT_ERROR_REFUSED = 8;
// the trace flag of the job
const u64 TRACE_FLAG = 0x4000;
// the type flags of a URL behind a NAT and of a public one
const u32 URL_TYPE_BEHIND_NAT = 1;
const u32 URL_TYPE_PUBLIC = 2;

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}

// (inline) the root transport of the network (null if there is none)
inline RootTransport* GetRootTransport()
{
    Network* pNetwork = Network::GetInstance();
    return pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
}

// (inline) the deadline of a timeout (0: none)
inline Time GetDeadline(s32 timeout)
{
    if (timeout == -1) {
        return Time(0);
    }
    return SystemClock::GetTime() + timeout;
}

// (inline) the call of the caller
inline CallContext* GetCallContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// 0x0033F754 (name is ours)
void nn::nex::JobConnectEndPoint::OnConnectCompleted(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext)
{
    ScopedCS lock(GetSchedulerLock());
    JobConnectEndPoint* pJob = static_cast<JobConnectEndPoint*>(pContext->m_pPointer);
    if (pJob->m_State == STATE_SUSPENDED) {
        pJob->Trace(pJob->m_TraceFlags);
        pJob->m_Result = result;
        pJob->SetState(STATE_READY);
        pJob->ReleaseRef();
    }
}

// 0x0033F820 (name is ours)
void nn::nex::JobConnectEndPoint::OnDisconnectCompleted(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext)
{
    ScopedCS lock(GetSchedulerLock());
    JobConnectEndPoint* pJob = static_cast<JobConnectEndPoint*>(pContext->m_pPointer);
    pJob->SetState(STATE_READY);
    pJob->ReleaseRef();
}

// 0x00387FF8 (name is ours)
void nn::nex::JobConnectEndPoint::Connect()
{
    StationURL& url = m_pCurrentURL->m_Value;
    ConnectionOrientedStream* pStream = nullptr;
    qList<ConnectionOrientedStream*>& streams = m_pManager->m_StreamTable.m_Streams;
    for (qList<ConnectionOrientedStream*>::Node* pNode = streams.GetFirst(); !streams.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value->ResponsibleForURL(&url)) {
            pStream = pNode->m_Value;
            break;
        }
    }
    if (pStream != nullptr) {
        EndPoint* pEndPoint = pStream->OpenEndPoint(&url);
        if (pEndPoint != nullptr) {
            pEndPoint->RegisterEventHandler(m_pManager->m_pHandler);
            m_pEndPoint = pEndPoint;
            if (m_IsRouting && __builtin_bswap32(m_pManager->m_Address.m_SockAddr.m_Address) != 0) {
                // the end point goes through the routing station
                pEndPoint->SetMaxSilenceTime(static_cast<u32>(-1));
                pEndPoint->DisableKeepAlive();
                Router* pRouter = GetRootTransport()->GetRouter();
                pRouter->m_pStream->m_RoutingTable.Add(*pEndPoint->m_URL.GetInetAddressInline(), m_pManager->m_Address, true);
            }
            UserContext context;
            context.m_pPointer = this;
            m_Result = m_pEndPoint->_Connect(m_pConnectData, m_pReplyData, &JobConnectEndPoint::OnConnectCompleted, context, m_Timeout);
            if (m_Result.m_Code >= 0) {
                AcquireRef();
                SetState(STATE_SUSPENDED);
                SetStepInline(&JobConnectEndPoint::ProcessConnectResult, "");
                return;
            }
            m_pEndPoint->m_pStream->CloseEndPoint(m_pEndPoint);
            m_pEndPoint = nullptr;
        }
    }
    NextURL();
}

// 0x0038828C (name is ours)
void nn::nex::JobConnectEndPoint::TryNextURL()
{
    if (Network::GetInstance() == nullptr) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    if (m_URLs.IsEnd(m_pCurrentURL)) {
        SetStepInline(&JobConnectEndPoint::SelectConnectionTechnique, "");
        return;
    }
    if (Network::GetInstance()->m_pNATTraversalEngine != nullptr) {
        InetAddress via;
        const InetAddress* pAddress = m_pCurrentURL->m_Value.GetInetAddressInline();
        if (!GetRootTransport()->GetRouter()->m_pStream->m_RoutingTable.Find(*pAddress, via) &&
            (!m_IsRouting || __builtin_bswap32(m_pManager->m_Address.m_SockAddr.m_Address) == 0)) {
            StationURL& url = m_pCurrentURL->m_Value;
            if ((g_UsesNATTraversalForRelay && url.GetParamValue(L"R", 0) != 0) ||
                ((url.GetParamValue(L"type", 0) & URL_TYPE_BEHIND_NAT) && (url.GetParamValue(L"type", 0) & URL_TYPE_PUBLIC))) {
                SetStepInline(&JobConnectEndPoint::StartNATTraversal, "");
                return;
            }
        }
    }
    SetStepInline(&JobConnectEndPoint::ResolveURL, "");
}

// 0x003884C8 slot 0x30 | fefates:bytes
void nn::nex::JobConnectEndPoint::CheckExceptions()
{
    if (GetCallContext(m_CallContextId) == nullptr || Core::GetInstance()->m_IsInterrupted || m_IsCancelled) {
        SetStepInline(&JobConnectEndPoint::CancelledJob, "");
    }
}

// 0x00388548 (name is ours)
void nn::nex::JobConnectEndPoint::FailJob()
{
    ReportNATTraversalResult(false);
    int code = m_IsFault8 ? RESULT_CONNECTION_REFUSED : RESULT_CONNECTION_FAILED;
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(code), true);
    }
    SetState(STATE_COMPLETED);
}

// 0x003885F0 (name is ours)
void nn::nex::JobConnectEndPoint::TryConnectDirect()
{
    m_pCurrentURL = m_URLs.GetFirst();
    m_IsRouting = false;
    if (m_URLs.IsEnd(m_pCurrentURL)) {
        SetStepInline(&JobConnectEndPoint::SelectConnectionTechnique, "");
    } else {
        SetStepInline(&JobConnectEndPoint::TryNextURL, "");
    }
}

// 0x00388694 (name is ours)
void nn::nex::JobConnectEndPoint::ResolveURL()
{
    m_ResolveContext.m_Deadline = GetDeadline(m_Timeout);
    if (!m_IsRouting) {
        ApplyUpdatedPort();
    }
    TransportAdapter::s_pInstance->ResolveAddress(&m_ResolveContext, &m_pCurrentURL->m_Value);
    SetStepInline(&JobConnectEndPoint::WaitForURLResolution, "");
}

// 0x00388768 (name is ours)
void nn::nex::JobConnectEndPoint::RetryURL()
{
    if (Network::GetInstance() == nullptr || Network::GetInstance()->m_pNATTraversalEngine == nullptr) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    StationURL unused;
    if (!ApplyUpdatedPort()) {
        if (m_OriginalPort != 0) {
            m_pCurrentURL->m_Value.SetPortNumberInline(m_OriginalPort);
            m_OriginalPort = 0;
        }
        m_pCurrentURL = m_pCurrentURL->m_pNext;
    }
    SetStepInline(&JobConnectEndPoint::TryNextURL, "");
}

// 0x00388870 (name is ours)
void nn::nex::JobConnectEndPoint::CancelledJob()
{
    if (m_pEndPoint != nullptr) {
        m_pEndPoint->m_pStream->CloseEndPoint(m_pEndPoint);
        m_pEndPoint = nullptr;
    }
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_CANCELLED, qResult(RESULT_CANCELLED), true);
    }
    SetState(STATE_COMPLETED);
}

// 0x003888FC (name is ours)
void nn::nex::JobConnectEndPoint::SucceededJob()
{
    ReportNATTraversalResult(true);
    if (m_pCurrentURL->m_Value.GetParamValue(L"CID", 0) != 0) {
        // the end point gets the connection id of the URL
        EndPoint* pEndPoint = m_pEndPoint;
        u32 connectionId = m_pCurrentURL->m_Value.GetParamValue(L"CID", 0);
        if (pEndPoint->m_pStream != nullptr) {
            if (pEndPoint->m_ConnectionId != 0) {
                pEndPoint->m_pStream->UnregisterConnectionID(pEndPoint->m_ConnectionId, pEndPoint);
            }
            if (connectionId != 0) {
                pEndPoint->m_pStream->RegisterConnectionID(connectionId, pEndPoint);
            }
        }
        pEndPoint->m_ConnectionId = connectionId;
    }
    *m_ppEndPoint = m_pEndPoint;
    m_pEndPoint = nullptr;
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(qResult::SUCCESS), true);
    }
    SetState(STATE_COMPLETED);
}

// 0x00388A08 (name is ours)
void nn::nex::JobConnectEndPoint::StartNATTraversal()
{
    if (Network::GetInstance() == nullptr || Network::GetInstance()->m_pNATTraversalEngine == nullptr) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    bool isWaiting;
    if (m_IsPassive) {
        // the peer probes: a reply of a probe until now is enough
        NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
        StationURL& url = m_pCurrentURL->m_Value;
        URLProbe* pProbe;
        s32 result = -1;
        {
            ScopedCS lock(GetSchedulerLock());
            pProbe = pEngine->m_FinishedProbes.FindProbe(url);
            if (pProbe != nullptr) {
                result = pProbe->m_Result;
            }
        }
        isWaiting = pProbe == nullptr || result == -1;
    } else {
        isWaiting = Network::GetInstance()->m_pNATTraversalEngine->PrepareNATTraversal(m_pCurrentURL->m_Value);
    }
    if (isWaiting) {
        m_ProbeDeadline = GetDeadline(RootTransport::s_CheckAndRelayTimeout);
        SetStepInline(&JobConnectEndPoint::WaitForNATTraversal, "");
    } else {
        SetStepInline(&JobConnectEndPoint::ResolveURL, "");
    }
}

// 0x00388C24 (name is ours)
void nn::nex::JobConnectEndPoint::WaitForNATTraversal()
{
    if (Network::GetInstance() == nullptr || Network::GetInstance()->m_pNATTraversalEngine == nullptr) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
    StationURL& url = m_pCurrentURL->m_Value;
    URLProbe* pProbe;
    s32 result = -1;
    {
        ScopedCS lock(GetSchedulerLock());
        pProbe = pEngine->m_FinishedProbes.FindProbe(url);
        if (pProbe != nullptr) {
            result = pProbe->m_Result;
        }
    }
    if (pProbe != nullptr && result != -1) {
        m_Rtt = result;
        SetStepInline(&JobConnectEndPoint::ResolveURL, "");
        return;
    }
    if (m_ProbeDeadline < SystemClock::GetTime() || (!m_IsPassive && pEngine->IsProbeFailed(url))) {
        NextURL();
        return;
    }
    SetState(STATE_WAITING);
    m_WaitTime = WAIT_TIME;
}

// 0x00388E20 slot 0x28 (name is ours)
void nn::nex::JobConnectEndPoint::vf_0x28()
{
    CancelJob();
}

// 0x00388E2C | fefates:bytes [tier B]
void nn::nex::JobConnectEndPoint::TryConnectViaRouting()
{
    m_pCurrentURL = m_URLs.GetFirst();
    m_IsRouting = true;
    SetStepInline(&JobConnectEndPoint::TryNextURL, "");
}

// 0x00388E90 | fefates:bytes [tier B]
void nn::nex::JobConnectEndPoint::WaitForURLResolution()
{
    switch (m_ResolveContext.m_State) {
    case CallContext::STATE_CALL_IN_PROGRESS:
        SetState(STATE_WAITING);
        m_WaitTime = WAIT_TIME;
        return;
    case CallContext::STATE_SUCCESS:
        SetStepInline(&JobConnectEndPoint::Connect, "");
        break;
    default:
        NextURL();
        break;
    }
    m_ResolveContext.Reset();
}

// 0x00388F8C slot 0x10 | fefates:bytes
void nn::nex::JobConnectEndPoint::TestSuspendedJobState()
{
    if (GetCallContext(m_CallContextId) != nullptr && Core::GetInstance()->m_IsInterrupted) {
        // nothing to do in this build
    }
}

// 0x00388FB8 (name is ours)
bool nn::nex::JobConnectEndPoint::ApplyUpdatedPort()
{
    if (Network::GetInstance()->m_pNATTraversalEngine == nullptr) {
        return false;
    }
    StationURL updated;
    StationURL& url = m_pCurrentURL->m_Value;
    if (Network::GetInstance()->m_pNATTraversalEngine->GetUpdatedURL(url, &updated) && url.GetPortNumberInline() != updated.GetPortNumberInline()) {
        m_OriginalPort = m_pCurrentURL->m_Value.GetPortNumberInline();
        m_pCurrentURL->m_Value.SetPortNumberInline(updated.GetPortNumberInline());
        return true;
    }
    return false;
}

// 0x003890E0 (name is ours)
void nn::nex::JobConnectEndPoint::ProcessConnectResult()
{
    if (m_Result.m_Code >= 0) {
        SetStepInline(&JobConnectEndPoint::SucceededJob, "");
        return;
    }
    if (m_pEndPoint->IsConnected()) {
        UserContext context;
        context.m_pPointer = this;
        qResult result = m_pEndPoint->_Disconnect(&JobConnectEndPoint::OnDisconnectCompleted, context, ConnectionManager::s_DisconnectTimeout);
        if (result.m_Code >= 0) {
            AcquireRef();
            SetState(STATE_SUSPENDED);
        }
    }
    SetStepInline(&JobConnectEndPoint::ProcessDisconnectResult, "");
}

// 0x003891FC (name is ours)
void nn::nex::JobConnectEndPoint::ProcessDisconnectResult()
{
    if (m_pEndPoint->GetSignaledFaultError() == FAULT_ERROR_REFUSED) {
        m_IsFault8 = true;
    }
    m_pEndPoint->m_pStream->CloseEndPoint(m_pEndPoint);
    m_pEndPoint = nullptr;
    if (Network::GetInstance() == nullptr) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    if (Network::GetInstance()->m_pNATTraversalEngine != nullptr) {
        SetStepInline(&JobConnectEndPoint::RetryURL, "");
        return;
    }
    NextURL();
}

// 0x0038934C | fefates:bytes-fuzzy [tier B]
void nn::nex::JobConnectEndPoint::ReportNATTraversalResult(bool isSucceeded)
{
    if (Network::GetInstance() == nullptr || Network::GetInstance()->m_pNATTraversalEngine == nullptr || m_IsPassive) {
        return;
    }
    NATTraversalEngine* pEngine = Network::GetInstance()->m_pNATTraversalEngine;
    u32 rvcid = 0;
    for (qList<StationURL>::Node* pNode = m_URLs.GetFirst(); !m_URLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        if (pNode->m_Value.GetParamValue(L"RVCID", 0) != 0) {
            rvcid = pNode->m_Value.GetParamValue(L"RVCID", 0);
            break;
        }
    }
    if (pEngine == nullptr || rvcid == 0) {
        return;
    }
    u32 cid = rvcid;
    bool isSucceededCopy = isSucceeded;
    u32 rtt = m_Rtt;
    if (pEngine->m_pRelayInterface != nullptr) {
        NATTraversalResult detail = pEngine->GetNATTraversalResult(rvcid);
        pEngine->m_pRelayInterface->ReportNATTraversalResultDetail(cid, isSucceededCopy, detail, rtt);
    }
}

// 0x00389450 | fefates:bytes [tier B]
void nn::nex::JobConnectEndPoint::SelectConnectionTechnique()
{
    if (m_Techniques.GetSize() == 0) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    SetStepInline(m_Techniques.front());
    m_Techniques.erase(m_Techniques.GetFirst());
}

// 0x00389530 slot 0x34 | fefates:bytes
void nn::nex::JobConnectEndPoint::Trace(unsigned long long flags)
{
    // (the URLs are traced in the debug build)
    for (qList<StationURL>::Node* pNode = m_URLs.GetFirst(); !m_URLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        // nothing to do
    }
}

// 0x00389548 (name is ours)
void nn::nex::JobConnectEndPoint::SortURLs()
{
    if (Network::GetInstance() == nullptr) {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
        return;
    }
    Network* pNetwork = Network::GetInstance();
    {
        ScopedCS lock(pNetwork->m_StationUrls.m_Lock);
        TransportAdapter::s_pInstance->SortAndFilterTargetURLs(pNetwork->m_StationUrls, &m_URLs);
    }
    if (m_URLs.GetSize() == 0 || m_Techniques.GetSize() == 0) {
        m_Timeout = 0;
    }
    SetStepInline(&JobConnectEndPoint::SelectConnectionTechnique, "");
}

// 0x00389644 slot 0x24 | fefates:bytes
void nn::nex::JobConnectEndPoint::CancelJob()
{
    if (m_IsCancelled) {
        return;
    }
    Trace(TRACE_FLAG);
    m_IsCancelled = true;
    if (m_pEndPoint != nullptr && m_pEndPoint->IsConnecting()) {
        m_pEndPoint->SetConnectionState(static_cast<EndPoint::_ConnectionState>(0));
    }
    if (m_pSubJob != nullptr) {
        m_pSubJob->CancelJob();
    }
}

// 0x003896CC
nn::nex::JobConnectEndPoint::JobConnectEndPoint(nn::nex::Job::JobType type, nn::nex::ConnectionManager* pManager, unsigned int callContextId, nn::nex::Buffer* pConnectData,
                                                nn::nex::Buffer* pReplyData, const nn::nex::qList<nn::nex::StationURL>& urls, nn::nex::EndPoint** ppEndPoint, unsigned int timeout,
                                                bool unknown)
    : StepSequenceJob(type, DebugString()), m_Use(&pManager->m_State), m_pEndPoint(nullptr), m_pManager(pManager), m_Unknown0x90(0), m_Unknown0x94(0), m_Timeout(timeout),
      m_Unknown0xB4(0), m_CallContextId(callContextId), m_pConnectData(pConnectData), m_pReplyData(pReplyData), m_ppEndPoint(ppEndPoint), m_ProbeDeadline(0), m_URLs(urls),
      m_IsRouting(false), m_OriginalPort(0), m_pCurrentURL(m_URLs.m_pHead), m_IsPassive(unknown), m_IsCancelled(false), m_pSubJob(nullptr), m_Rtt(0), m_IsFault8(false)
{
    m_TraceFlags = TRACE_FLAG;
    if (m_pConnectData != nullptr) {
        m_pConnectData->AcquireRef();
    }
    if (m_pReplyData != nullptr) {
        m_pReplyData->AcquireRef();
    }
    m_Result = qResult(qResult::SUCCESS);
    if (m_pManager->m_Unknown0x74) {
        m_Techniques.push_back(Step(&JobConnectEndPoint::TryConnectDirect, ""));
    }
    if (m_pManager->m_IsRoutingEnabled && (__builtin_bswap32(m_pManager->m_Address.m_SockAddr.m_Address) != 0 || !m_pManager->m_Unknown0x74)) {
        m_Techniques.push_back(Step(&JobConnectEndPoint::TryConnectViaRouting, ""));
    }
    if (m_URLs.GetSize() != 0) {
        SetStepInline(&JobConnectEndPoint::SortURLs, "");
    } else {
        SetStepInline(&JobConnectEndPoint::FailJob, "");
    }
}

// 0x00389A60 slot 0x00 | fefates:bytes
// 0x00389A30 (deleting dtor)
nn::nex::JobConnectEndPoint::~JobConnectEndPoint()
{
    if (m_pConnectData != nullptr) {
        m_pConnectData->ReleaseRef();
    }
    if (m_pReplyData != nullptr) {
        m_pReplyData->ReleaseRef();
    }
}

// 0x0082FA24
template <>
DECOMP_NOINLINE void qList<StepSequenceJob::Step>::push_back(const StepSequenceJob::Step& value)
{
    PushBackInline(value);
}

} // namespace nex
} // namespace nn
