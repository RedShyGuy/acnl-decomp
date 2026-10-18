#include "nn/nex/nex_JobConnectSecureEndPoint.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_BitStream.h"
#include "nn/nex/nex_Buffer.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_JobConnectEndPoint.h"
#include "nn/nex/nex_KerberosEncryption.h"
#include "nn/nex/nex_Platform.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_SecureEndPoint.h"
#include "nn/nex/nex_SecureStream.h"
#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_Ticket.h"
#include "nn/nex/nex_TicketManager.h"

namespace nn {
namespace nex {
// the result of RequestConnectionData that nobody reads (name is ours)
// 0x0096CE19
bool g_RequestConnectionDataResult;

namespace {
// the URL has no principal and no connection id
const int RESULT_INVALID_URL = static_cast<int>(0x80050003);
// the connection of the end point below failed
const int RESULT_CONNECTION_FAILED = static_cast<int>(0x80050001);
// the reply of the connection has another number
const int RESULT_INVALID_REPLY = static_cast<int>(0x8005000A);
// a call could not be sent
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}

// (inline) the secure stream of the end point
inline SecureStream* GetStream(SecureEndPoint* pEndPoint)
{
    return static_cast<SecureStream*>(pEndPoint->m_pStream);
}
} // namespace

// 0x00176598 (name is ours)
void nn::nex::JobConnectSecureEndPoint::OnCallCompleted(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext)
{
    JobConnectSecureEndPoint* pJob = static_cast<JobConnectSecureEndPoint*>(pUserContext->m_pPointer);
    ScopedCS lock(GetSchedulerLock());
    if (pJob->m_State == STATE_SUSPENDED) {
        pJob->SetReadyState();
    }
}

// 0x003B2A8C | mk7dlp:callseq [tier A]
void nn::nex::JobConnectSecureEndPoint::BuildListURLs(nn::nex::qList<nn::nex::StationURL>* pURLs)
{
    for (qList<ConnectionData>::Node* pNode = m_URLs.GetFirst(); !m_URLs.IsEnd(pNode); pNode = pNode->m_pNext) {
        StationURL url(pNode->m_Value.m_StationUrl);
        url.SetConnectionID(pNode->m_Value.m_ConnectionID);
        pURLs->PushBackInline(url);
    }
}

// 0x003B2BAC | fefates:bytes-fuzzy [tier B]
void nn::nex::JobConnectSecureEndPoint::PerformConnect()
{
    if (m_URLs.GetSize() == 0) {
        m_Result = qResult(RESULT_INVALID_URL);
        m_Step = STEP_DONE;
        SetToComplete();
        return;
    }
    ConnectionManager* pManager = GetStream(m_pEndPoint)->m_pCredentials->m_pStreamManager->m_pConnectionManager;
    InitializeBufferRequest();
    if (m_pReply != nullptr) {
        delete m_pReply;
    }
    m_pReply = new BitStream();
    qList<StationURL> urls;
    BuildListURLs(&urls);
    UserContext context;
    context.m_pPointer = this;
    m_ConnectContext.RegisterCompletionCallback(&JobConnectSecureEndPoint::OnConnectCompleted, context, false);
    if (pManager->ConnectImpl(&m_ConnectContext, m_pRequest->m_pBuffer, m_pReply->m_pBuffer, urls, &m_pConnectedEndPoint, Time::ConvertDeadlineToTimeout(m_Deadline), false,
                              &m_pConnectJob)) {
        m_pConnectJob->AcquireRef();
        m_Step = STEP_PROCESS_CONNECT_RESULT;
        SetToSuspended();
    } else {
        m_Result = qResult(RESULT_CALL_FAILED);
        m_Step = STEP_PROCESS_CONNECT_RESULT;
        SetToRunning();
    }
}

// 0x003B2D58 | fefates:bytes-fuzzy [tier B]
void nn::nex::JobConnectSecureEndPoint::ProcessConnectResult()
{
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->ReleaseRef();
        m_pConnectJob = nullptr;
    }
    bool isSuccess = true;
    if (m_Result.Equals(isSuccess)) {
        m_pReply->AdjustLength();
        BitStream* pReply = m_pReply;
        u32 random = m_Random;
        bool isValid = false;
        {
            BitStream body;
            *pReply >> body;
            if (!pReply->m_IsError) {
                u32 value;
                body.Extract(reinterpret_cast<u8*>(&value), 4, 1);
                if (!body.m_IsError && value == random + 1) {
                    isValid = true;
                }
            }
        }
        if (!isValid) {
            m_Result = qResult(RESULT_INVALID_REPLY);
        }
        if (m_pEndPoint != nullptr) {
            m_pEndPoint->SetEncryptionKey(m_pTicket->m_SessionKey);
        }
    }
    m_Step = STEP_DONE;
    SetToComplete();
}

// 0x003B2EAC | fefates:bytes-fuzzy [tier B]
void nn::nex::JobConnectSecureEndPoint::RequestConnectionData()
{
    SecureStream* pStream = GetStream(m_pEndPoint);
    u32 sourceId = pStream->m_pCredentials->m_PrincipalId;
    if (m_PrincipalId != 0 && m_pTicket == nullptr && m_TicketContext.m_State == CallContext::STATE_AVAILABLE) {
        UserContext context;
        context.m_pPointer = this;
        m_TicketContext.RegisterCompletionCallback(&JobConnectSecureEndPoint::OnCallCompleted, context, true);
        if (!pStream->m_pAuthenticationClient->m_pTicketManager->AcquireTicket(&m_TicketContext, sourceId, m_PrincipalId, &m_pTicket)) {
            m_Result = qResult(RESULT_CALL_FAILED);
            m_Step = STEP_DONE;
            SetToComplete();
            return;
        }
    }
    if (m_URLs.GetSize() == 0 && m_URLContext.m_State == CallContext::STATE_AVAILABLE) {
        UserContext context;
        context.m_pPointer = this;
        m_URLContext.RegisterCompletionCallback(&JobConnectSecureEndPoint::OnCallCompleted, context, true);
        u32 connectionId = m_ConnectionId;
        u32 principalId = m_PrincipalId;
        if (!pStream->m_pSecureConnectionClient->m_Protocol.CallRequestConnectionData(&m_URLContext, &g_RequestConnectionDataResult, connectionId, principalId, &m_URLs)) {
            m_Result = qResult(RESULT_CALL_FAILED);
            m_Step = STEP_DONE;
            SetToComplete();
            return;
        }
    }
    if (m_TicketContext.m_State == CallContext::STATE_CALL_IN_PROGRESS || m_URLContext.m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        m_Step = STEP_CHECK_CONNECTION_DATA;
        SetToSuspended();
        return;
    }
    m_Result = qResult(RESULT_CALL_FAILED);
    m_Step = STEP_DONE;
    SetToComplete();
}

// 0x003B3070 | fefates:bytes [tier B]
void nn::nex::JobConnectSecureEndPoint::InitializeBufferRequest()
{
    SecureStream* pStream = GetStream(m_pEndPoint);
    if (m_pRequest != nullptr) {
        delete m_pRequest;
    }
    m_pRequest = new BitStream();
    AuthenticationClient* pAuthentication = pStream->m_pAuthenticationClient;
    Ticket* pTicket = m_pTicket;
    BitStream* pRequest = m_pRequest;
    Credentials* pCredentials = pAuthentication->m_pDefaultCredentials;
    *pRequest << pTicket->m_Data;
    m_Random = Platform::GetRandomNumber(0xFFFFFFFF);
    BitStream body;
    u32 principalId = pAuthentication->m_PrincipalId;
    body.Append(reinterpret_cast<const u8*>(&principalId), 4, 1);
    u32 connectionId;
    if (pCredentials != nullptr) {
        connectionId = pCredentials->m_ConnectionId;
        body.Append(reinterpret_cast<const u8*>(&connectionId), 4, 1);
    } else {
        connectionId = 0;
        body.Append(reinterpret_cast<const u8*>(&connectionId), 4, 1);
    }
    body.Append(reinterpret_cast<const u8*>(&m_Random), 4, 1);
    Buffer encrypted;
    pAuthentication->m_pKerberos->Encrypt(*body.m_pBuffer, &encrypted, pTicket->m_SessionKey);
    *pRequest << encrypted;
}

// 0x003B31CC (name is ours)
void nn::nex::JobConnectSecureEndPoint::OnConnectCompleted(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext)
{
    JobConnectSecureEndPoint* pJob = static_cast<JobConnectSecureEndPoint*>(pUserContext->m_pPointer);
    ScopedCS lock(GetSchedulerLock());
    if (pContext->m_State != CallContext::STATE_SUCCESS) {
        pJob->m_Result = qResult(RESULT_CONNECTION_FAILED);
    } else {
        // the secure end point is above the connected one
        SecureEndPoint* pEndPoint = pJob->m_pEndPoint;
        EndPoint* pConnected = pJob->m_pConnectedEndPoint;
        pEndPoint->m_pEndPoint = pConnected;
        pConnected->m_pOwner = pEndPoint;
        pConnected->RegisterEventHandler(pEndPoint->m_pEventHandler);
        pJob->m_pConnectedEndPoint->SetPrincipalID(pJob->m_PrincipalId);
        pJob->m_pEndPoint->SetPrincipalID(pJob->m_PrincipalId);
        pJob->m_pEndPoint->SetConnectionID(pJob->m_pConnectedEndPoint->m_ConnectionId);
    }
    pJob->SetReadyState();
}

// 0x003B32A8 slot 0x0C | fefates:callseq
void nn::nex::JobConnectSecureEndPoint::Execute()
{
    do {
        switch (m_Step) {
        case STEP_PARSE_URL:
            ParseURL();
            m_Step = STEP_CHECK_CONNECTION_DATA;
            break;
        case STEP_REQUEST_CONNECTION_DATA:
            RequestConnectionData();
            break;
        case STEP_CHECK_CONNECTION_DATA:
            if (m_PrincipalId != 0 && m_URLs.GetSize() != 0 && m_pTicket != nullptr) {
                m_Step = STEP_CONNECT;
            } else {
                m_Step = STEP_REQUEST_CONNECTION_DATA;
            }
            break;
        case STEP_CONNECT:
            PerformConnect();
            break;
        case STEP_PROCESS_CONNECT_RESULT:
            ProcessConnectResult();
            break;
        }
    } while (m_State == STATE_RUNNING);
    if (m_State == STATE_COMPLETED && m_pCallback != nullptr) {
        m_pCallback(m_pEndPoint, m_Result, &m_Context);
    }
}

// 0x003B33A0 | mk7dlp:callseq [tier A]
void nn::nex::JobConnectSecureEndPoint::ParseURL()
{
    m_ConnectionId = m_URL.GetConnectionID();
    m_PrincipalId = m_URL.GetPrincipalID();
    bool isValid = m_URL.GetAddress().GetLength() != 0 && m_URL.GetPortNumber() != 0 && m_URL.GetStreamID() != 0;
    if (isValid) {
        // the URL itself is the connection data
        StationURL url;
        url.SetInetAddress(m_URL.GetInetAddress());
        url.SetStreamID(m_URL.GetStreamID());
        ConnectionData data;
        data.m_StationUrl = url;
        data.m_ConnectionID = m_ConnectionId;
        m_URLs.PushBackInline(data);
    }
    if (m_PrincipalId == 0 && m_ConnectionId == 0) {
        m_Result = qResult(RESULT_INVALID_URL);
        m_Step = STEP_DONE;
        SetToComplete();
    }
}

// 0x003B35E0 slot 0x24 | fefates:bytes
void nn::nex::JobConnectSecureEndPoint::CancelJob()
{
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->CancelJob();
    }
}

// 0x003B35FC | mk7dlp:callseq [tier A]
nn::nex::JobConnectSecureEndPoint::JobConnectSecureEndPoint(nn::nex::SecureEndPoint* pEndPoint, const nn::nex::StationURL* pURL, nn::nex::Buffer* pConnectData,
                                                            nn::nex::Buffer* pReplyData, nn::nex::EndPoint::CompletionCallback pCallback, const nn::nex::UserContext& context,
                                                            unsigned int timeout)
    : Job(static_cast<JobType>(2), DebugString()), m_TicketContext(nullptr), m_URLContext(nullptr), m_pEndPoint(pEndPoint), m_URL(*pURL), m_pReplyData(pReplyData),
      m_pCallback(pCallback), m_Context(context), m_Deadline(0), m_pConnectJob(nullptr)
{
    m_Step = STEP_PARSE_URL;
    m_Deadline = Time::ConvertTimeoutToDeadline(timeout);
    m_Result = qResult(qResult::SUCCESS);
    m_pTicket = nullptr;
    m_pRequest = nullptr;
    m_pReply = nullptr;
    m_PrincipalId = 0;
    m_ConnectionId = 0;
    m_pConnectedEndPoint = nullptr;
    if (m_Context.m_pPointer != nullptr) {
        static_cast<JobConnectEndPoint*>(m_Context.m_pPointer)->m_pSubJob = this;
    }
}

// 0x003B3788 slot 0x00
// 0x003B3778 (deleting dtor)
nn::nex::JobConnectSecureEndPoint::~JobConnectSecureEndPoint()
{
    ScopedCS lock(GetSchedulerLock());
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->ReleaseRef();
        m_pConnectJob = nullptr;
    }
    if (m_pTicket != nullptr) {
        GetStream(m_pEndPoint)->m_pAuthenticationClient->m_pTicketManager->ReleaseTicket(m_pTicket);
        m_pTicket = nullptr;
    }
    if (m_pRequest != nullptr) {
        delete m_pRequest;
        m_pRequest = nullptr;
    }
    if (m_pReply != nullptr) {
        delete m_pReply;
        m_pReply = nullptr;
    }
}

} // namespace nex
} // namespace nn
