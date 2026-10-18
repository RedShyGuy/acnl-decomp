#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_ConnectionOrientedStream.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_JobConnectEndPoint.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Router.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x0096C9B0 (name is ours)
u32 ConnectionManager::s_DisconnectTimeout = 15000;

namespace {
// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// the router of the transport of the network
inline Router* GetRouter()
{
    Network* pNetwork = Network::GetInstance();
    return (pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr)->GetRouter();
}
} // namespace

// 0x0038625C
bool nn::nex::ConnectionManager::Disconnect(nn::nex::CallContext* pContext, nn::nex::EndPoint* pEndPoint)
{
    ScopedCS lock(GetSchedulerLock());
    if (!pContext->InitiateCall()) {
        return false;
    }
    UserContext context;
    context.m_Value = pContext->m_Id;
    qResult result = pEndPoint->_Disconnect(DisconnectCompletion, context, s_DisconnectTimeout);
    if (result.m_Code < 0) {
        pEndPoint->m_pStream->CloseEndPoint(pEndPoint);
        return false;
    }
    return true;
}

// 0x00386334
bool nn::nex::ConnectionManager::ConnectImpl(nn::nex::CallContext* pContext, nn::nex::Buffer* pConnectData, nn::nex::Buffer* pReplyData,
                                             const nn::nex::qList<nn::nex::StationURL>& urls, nn::nex::EndPoint** ppEndPoint, unsigned int timeout, bool unknown,
                                             nn::nex::JobConnectEndPoint** ppJob)
{
    ScopedCS lock(GetSchedulerLock());
    if (!pContext->InitiateCall()) {
        return false;
    }
    JobConnectEndPoint* pJob = new JobConnectEndPoint(static_cast<Job::JobType>(m_JobType), this, pContext->m_Id, pConnectData, pReplyData, urls, ppEndPoint, timeout, unknown);
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
    if (ppJob != nullptr) {
        *ppJob = pJob;
    }
    return true;
}

// 0x00386430
void nn::nex::ConnectionManager::AttachStream(nn::nex::ConnectionOrientedStream* pStream, bool unknown)
{
    pStream->RegisterEventHandler(this);
    m_StreamTable.m_Streams.push_back(pStream);
}

// 0x00386518 slot 0x18
bool nn::nex::ConnectionManager::ConnectionRequest(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL, nn::nex::Buffer* pData,
                                                   nn::nex::EndPoint* pEndPoint)
{
    if (PseudoSingleton::s_IsContextUsed) {
        PseudoSingleton::SetContext(m_Context);
    }
    if (!m_IsAccepting) {
        return false;
    }
    if (!FilterConnectionRequest(pStream, pURL, pData, pEndPoint)) {
        return false;
    }
    pEndPoint->RegisterEventHandler(m_pHandler);
    return true;
}

// 0x003865A4 slot 0x1C | mk7dlp:callseq
bool nn::nex::ConnectionManager::FilterConnectionRequest(nn::nex::ConnectionOrientedStream* pStream, const nn::nex::StationURL* pURL, nn::nex::Buffer* pData,
                                                         nn::nex::EndPoint* pEndPoint)
{
    if (m_pFilter == nullptr) {
        return true;
    }
    Buffer reply;
    if (!m_pFilter(m_FilterContext, pData, &reply, pEndPoint)) {
        return false;
    }
    *pData = reply;
    return true;
}

// 0x00386620
void nn::nex::ConnectionManager::RegisterConnectionFilter(ConnectionFilter pFilter, nn::nex::UserContext context)
{
    m_pFilter = pFilter;
    m_FilterContext = context;
}

// 0x00386630 | fefates:bytes [tier B]
bool nn::nex::ConnectionManager::Connect(nn::nex::CallContext* pContext, const nn::nex::StationURL& url, nn::nex::EndPoint** ppEndPoint, unsigned int timeout,
                                         nn::nex::JobConnectEndPoint** ppJob)
{
    qList<StationURL> urls;
    urls.push_back(url);
    return ConnectImpl(pContext, nullptr, nullptr, urls, ppEndPoint, timeout, false, ppJob);
}

// 0x0038678C | fefates:bytes-fuzzy [tier B]
nn::nex::ConnectionManager::ConnectionManager(nn::nex::EndPointEventHandler* pHandler, nn::nex::Job::JobType jobType, bool unknown, bool isRoutingEnabled)
    : m_State(String(L"ConnectionManager")), m_pFilter(nullptr), m_pHandler(pHandler), m_Unknown0x74(unknown), m_IsRoutingEnabled(isRoutingEnabled), m_JobType(jobType)
{
    m_FilterContext.m_Value = 0;
    m_IsAccepting = false;
    m_Context = PseudoSingleton::GetCurrentContext();
    m_State.Initialize();
    GetRouter()->m_pStream->m_RoutingTable.Update();
    GetRouter()->Enable(false);
    GetRouter()->Enable(m_IsRoutingEnabled);
}

// 0x00386A98 slot 0x00
// 0x00386A2C (deleting dtor)
nn::nex::ConnectionManager::~ConnectionManager()
{
    // nothing to do: the members are destroyed by the compiler
}

// 0x0039AE34 slot 0x14
void nn::nex::ConnectionManager::vf_0x14()
{
    // nothing to do
}

// 0x0039AE38 slot 0x0C
void nn::nex::ConnectionManager::vf_0x0C()
{
    // nothing to do
}

// 0x0039AE3C slot 0x10
bool nn::nex::ConnectionManager::SwitchToRoutingConnection(nn::nex::EndPoint* pEndPoint)
{
    return false;
}

// 0x0039AE44 slot 0x08
void nn::nex::ConnectionManager::Receive(nn::nex::ConnectionOrientedStream* pStream, nn::nex::Buffer* pData, const nn::nex::StationURL* pURL)
{
    // nothing to do
}

// 0x005290F4 (name is ours)
void nn::nex::ConnectionManager::DisconnectCompletion(nn::nex::EndPoint* pEndPoint, nn::nex::qResult result, const nn::nex::UserContext* pContext)
{
    ScopedCS lock(GetSchedulerLock());
    CallContext* pCallContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(pContext->m_Value);
    if (pCallContext != nullptr) {
        pCallContext->SetStateImpl(result.m_Code >= 0 ? CallContext::STATE_SUCCESS : CallContext::STATE_FAILURE, result, true);
    }
    pEndPoint->m_pStream->CloseEndPoint(pEndPoint);
}

// 0x0072C1E4 | fefates:bytes [tier B]
bool nn::nex::ConnectionManager::IsTerminated() const
{
    return m_State.m_State == SystemComponent::STATE_TERMINATED;
}

} // namespace nex
} // namespace nn
