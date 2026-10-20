#include "nn/nex/nex_JobNATCheckBase.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_JobNameResolve.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
namespace {
// the results of the check (the names are ours)
const int RESULT_CHECK_SUCCESS = 0x10001;
const int RESULT_FAILURE = static_cast<int>(0x80010001);
// the types of the requests (the replies come back with the same type)
const u32 TYPE_ADDRESS = 0;
const u32 TYPE_MAPPING = 101;
const u32 TYPE_MAPPING_PORT = 102;
const u32 TYPE_FILTERING = 103;
// the port of the address requests and the number of the requests of each type
const u16 NAT_CHECK_PORT = 33335;
const int REQUEST_COUNT = 5;
// the time for the replies (ms) and between the tests of a waiting step
const int REPLY_TIME = 2500;
const int WAIT_TIME = 10;
// the attempts to bind the socket to a port of the root transport
const u32 BIND_ATTEMPTS = 5;
// the NAT maps the address per destination
const u8 NAT_MAPPING_DEPENDENT = 2;

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

// the attempts of a check (name is ours)
// 0x0096CE28
u32 g_MaxNATCheckAttempts = 3;

// 0x003796CC (name is ours)
void nn::nex::JobNATCheckBase::Finish()
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    m_Socket.Close();
    if (pContext == nullptr) {
        if (m_ResolveContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
            m_ResolveContext.Cancel(CallContext::STATE_CANCELLED);
        }
        SetToComplete();
        return;
    }
    if (m_ResolveContext.m_State == CallContext::STATE_FAILURE) {
        pContext->SignalFailure(m_ResolveContext.m_Result);
        SetToComplete();
        return;
    }
    if (m_ResolveContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        m_ResolveContext.Cancel(CallContext::STATE_CANCELLED);
    }
    qResult result = GetResult();
    if (result) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_CHECK_SUCCESS), true);
        SetToComplete();
        return;
    }
    if (++m_RetryCount < g_MaxNATCheckAttempts) {
        SetStep(Step(&JobNATCheckBase::Start, ""));
        return;
    }
    pContext->SignalFailure(result);
    SetToComplete();
}

// 0x0037986C (name is ours)
void nn::nex::JobNATCheckBase::ReceiveReplies()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    if (m_Deadline < Time::GetTime()) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    // the requests are sent until one fails
    while (m_Requests.GetSize() != 0) {
        Request& request = m_Requests.front();
        bool isSent = m_Socket.SendTo(reinterpret_cast<u8*>(&request.m_Message), sizeof(NATCheckMessage), &request.m_Address) == Socket::RESULT_SUCCESS;
        m_Requests.erase(m_Requests.GetFirst());
        if (!isSent) {
            break;
        }
    }
    u32 size = 0;
    if (m_Socket.RecvFrom(reinterpret_cast<u8*>(&m_Received), sizeof(m_Received), &size, &m_ReplyAddress) == Socket::RESULT_SUCCESS && size == sizeof(m_Received)) {
        m_Received.ToHostByteOrder();
        u32 index = m_Received.m_Type - NATCheckMessage::TYPE_REPLY_MIN;
        if (index < NATCheckMessage::TYPE_REPLY_NUM) {
            m_Replies[index].m_Message = m_Received;
            m_Replies[index].m_Count++;
        }
    }
    if (IsFinished()) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    SetToWaiting(WAIT_TIME);
}

// 0x00379A8C (name is ours)
void nn::nex::JobNATCheckBase::WaitForURLs()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    switch (m_ResolveContext.m_State) {
    case CallContext::STATE_CALL_IN_PROGRESS:
        SetToWaiting(WAIT_TIME);
        return;
    case CallContext::STATE_SUCCESS:
        SetStep(Step(&JobNATCheckBase::StartCheck, ""));
        return;
    case CallContext::STATE_FAILURE:
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    default:
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
}

// 0x00379B6C (name is ours)
void nn::nex::JobNATCheckBase::Start()
{
    if (GetRootTransport() == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    if (GetCallContext(m_CallContextId) == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    for (u32 i = 0; i < NATCheckMessage::TYPE_REPLY_NUM; i++) {
        m_Replies[i] = Reply();
    }
    if (GetRootTransport() != nullptr && GetRootTransport()->m_AreNATCheckURLsReady) {
        SetStep(Step(&JobNATCheckBase::StartCheck, ""));
        return;
    }
    // the URLs of the servers are resolved first
    m_ResolveContext.Reset();
    if (!m_ResolveContext.InitiateCall()) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    JobNameResolve* pJob = new JobNameResolve(g_NameResolutionThreadPriority, g_NameResolutionCoreNo, &RootTransport::InitializeNATCheckURLs);
    pJob->Initialize(m_ResolveContext.m_Id);
    Scheduler::GetInstanceInline()->Queue(pJob, true);
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ResolveContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ResolveContext, new Step(&JobNATCheckBase::WaitForURLs, ""));
}

// 0x00379DEC | fefates:callgraph [tier C]
bool nn::nex::JobNATCheckBase::InitializeSocket()
{
    if (GetRootTransport() == nullptr || !GetRootTransport()->m_AreNATCheckURLsReady) {
        return false;
    }
    if (!m_Socket.Open(true)) {
        return false;
    }
    if (m_RetryCount != 0) {
        m_LocalAddress.SetPortNumber(0);
    }
    if (m_LocalAddress.GetPortNumber() != 0 && m_Socket.Bind(&m_LocalAddress, true)) {
        return true;
    }
    for (u32 i = 0; i < BIND_ATTEMPTS; i++) {
        m_LocalAddress.SetPortNumber(RootTransport::GetNextPortNumber());
        if (m_Socket.Bind(&m_LocalAddress, true)) {
            return true;
        }
    }
    return false;
}

// 0x00393374 slot 0x34 (name is ours)
void nn::nex::JobCheckPublicAddress::StartCheck()
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    if (!InitializeSocket()) {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
        SetToComplete();
        return;
    }
    SetStep(Step(&JobCheckPublicAddress::SendRequests, ""));
}

// 0x00393458 (name is ours)
void nn::nex::JobCheckPublicAddress::SendRequests()
{
    if (GetRootTransport() == nullptr || !GetRootTransport()->m_AreNATCheckURLsReady) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    if (GetCallContext(m_CallContextId) == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    qVector<StringStream*>& urls = GetRootTransport()->m_NATCheckURLs;
    for (int i = 0; i < REQUEST_COUNT; i++) {
        InetAddress server(StationURL(urls.m_pBegin[0]->CStr()).GetInetAddress()->GetAddress(), NAT_CHECK_PORT);
        m_Requests.push_back(Request(server, TYPE_ADDRESS));
    }
    for (int i = 0; i < REQUEST_COUNT; i++) {
        // a NAT that maps per destination is asked through the second server
        bool usesSecond;
        {
            NATProperties properties = GetRootTransport()->GetNATProperties();
            usesSecond = properties.GetNATMapping() == NAT_MAPPING_DEPENDENT && urls.size() > 1;
        }
        if (usesSecond) {
            StationURL url(urls.m_pBegin[1]->CStr());
            m_Requests.push_back(Request(*url.GetInetAddress(), TYPE_MAPPING));
        } else {
            StationURL url(urls.m_pBegin[0]->CStr());
            m_Requests.push_back(Request(*url.GetInetAddress(), TYPE_MAPPING));
        }
    }
    m_Deadline = Time::GetTime() + REPLY_TIME;
    SetStep(Step(&JobNATCheckBase::ReceiveReplies, ""));
}

// 0x003986C4 slot 0x34 (name is ours)
void nn::nex::JobCheckNATProperties::StartCheck()
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    if (!InitializeSocket()) {
        pContext->SetStateImpl(CallContext::STATE_FAILURE, qResult(RESULT_FAILURE), false);
        SetToComplete();
        return;
    }
    SetStep(Step(&JobCheckNATProperties::SendRequests, ""));
}

// 0x003987A8 (name is ours)
void nn::nex::JobCheckNATProperties::SendRequests()
{
    if (GetRootTransport() == nullptr || !GetRootTransport()->m_AreNATCheckURLsReady) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    if (GetCallContext(m_CallContextId) == nullptr) {
        SetStep(Step(&JobNATCheckBase::Finish, ""));
        return;
    }
    qVector<StringStream*>& urls = GetRootTransport()->m_NATCheckURLs;
    for (int i = 0; i < REQUEST_COUNT; i++) {
        InetAddress server(StationURL(urls.m_pBegin[0]->CStr()).GetInetAddress()->GetAddress(), NAT_CHECK_PORT);
        m_Requests.push_back(Request(server, TYPE_ADDRESS));
    }
    for (int i = 0; i < REQUEST_COUNT; i++) {
        StationURL url(urls.m_pBegin[0]->CStr());
        m_Requests.push_back(Request(*url.GetInetAddress(), TYPE_MAPPING));
    }
    for (int i = 0; i < REQUEST_COUNT; i++) {
        StationURL url(urls.m_pBegin[0]->CStr());
        m_Requests.push_back(Request(*url.GetInetAddress(), TYPE_MAPPING_PORT));
    }
    for (int i = 0; i < REQUEST_COUNT; i++) {
        StationURL url(urls.m_pBegin[2]->CStr());
        m_Requests.push_back(Request(*url.GetInetAddress(), TYPE_FILTERING));
    }
    m_Deadline = Time::GetTime() + REPLY_TIME;
    SetStep(Step(&JobNATCheckBase::ReceiveReplies, ""));
    m_StartTime = Time::GetTime();
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0082F780
template <>
DECOMP_NOINLINE void list<nn::nex::JobNATCheckBase::Request, nn::nex::MemAllocator<nn::nex::JobNATCheckBase::Request> >::push_back(const nn::nex::JobNATCheckBase::Request& value)
{
    PushBackInline(value);
}
} // namespace std
