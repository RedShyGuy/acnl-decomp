#include "nn/nex/nex_JobBackEndServicesLogin.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_ClientProtocolRequestBroker.h"
#include "nn/nex/nex_ClientStreamManager.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_PRUDPStream.h"
#include "nn/nex/nex_JobConnectEndPoint.h"
#include "nn/nex/nex_JobLogin.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_SecureStream.h"
#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_StringStream.h"

#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
// RendezVous::ConnectionFailure (the secure stream has credentials already)
const int RESULT_CONNECTION_FAILURE = static_cast<int>(0x80030001);
// the session id of the stations of the servers
const int SERVER_SESSION_ID = 1;
// the type of the station URLs of the servers (public)
const int SERVER_URL_TYPE = 2;

// (inline) the secure stream of the credentials
inline SecureStream* GetSecureStream(const Credentials* pCredentials)
{
    return static_cast<SecureStream*>((pCredentials != nullptr && pCredentials->m_pStreamManager != nullptr) ? pCredentials->m_pStreamManager->m_pSecureStream : nullptr);
}

// (inline) the connection manager of the credentials
inline ConnectionManager* GetConnectionManager(const Credentials* pCredentials)
{
    return (pCredentials != nullptr ? pCredentials->m_pStreamManager : nullptr)->m_pConnectionManager;
}
} // namespace

// 0x003AF97C | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::CompleteJob(nn::nex::qResult result)
{
    m_Result = result;
    if (!m_Result && m_pBackEndServices->m_pCredentials != nullptr) {
        m_CallContext.Reset();
        if (m_pBackEndServices->LogoutImpl(&m_CallContext, m_pBackEndServices->m_pCredentials)) {
            SetToSuspended();
            ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLogin::CompleteLogout, ""));
            return;
        }
    }
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        if (m_pResult != nullptr) {
            *m_pResult = m_Result;
        }
        if (m_Result) {
            *m_ppCredentials = m_pBackEndServices->m_pCredentials;
            pContext->SignalSuccess(m_Result);
        } else {
            pContext->SignalFailure(m_Result);
        }
    }
    SetToComplete();
}

// 0x003AFACC (name is ours)
void nn::nex::JobBackEndServicesLogin::LoginToAuthenticationService()
{
    if (GetSecureStream(m_pBackEndServices->m_pCredentials)->m_pCredentials != nullptr) {
        CompleteJob(qResult(RESULT_CONNECTION_FAILURE));
        return;
    }
    AuthenticationClient* pClient = m_pBackEndServices->GetAuthenticationClient();
    m_ProtocolCallContext.m_Deadline = m_Deadline;
    if (!pClient->Login(&m_ProtocolCallContext, &m_LoginResult, m_Name, m_pPassword, &m_ConnectionData, m_pServerName, m_pData)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobBackEndServicesLogin::ProcessAuthenticationResult, ""));
}

// 0x003AFC10 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::RegisterURLs()
{
    m_ProtocolCallContext.Reset();
    m_ProtocolCallContext.m_Deadline = m_Deadline;
    SecureConnectionClient* pClient = m_pBackEndServices->GetSecureConnectionClient();
    bool isCalled;
    {
        qList<StationURL> urls;
        pClient->PrepareURLs(urls);
        if (g_LoginCustomData.m_pObject != nullptr) {
            isCalled = pClient->m_Protocol.CallRegisterEx(&m_ProtocolCallContext, &m_LoginResult, urls, &pClient->m_pDefaultCredentials->m_ConnectionId, &m_URL,
                                                          g_LoginCustomData);
        } else {
            isCalled = pClient->m_Protocol.CallRegister(&m_ProtocolCallContext, &m_LoginResult, urls, &pClient->m_pDefaultCredentials->m_ConnectionId, &m_URL);
        }
    }
    if (!isCalled) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobBackEndServicesLogin::ProcessRegistrationResult, ""));
}

// 0x003AFD58 (name is ours)
void nn::nex::JobBackEndServicesLogin::ProcessRegistrationResult()
{
    if (m_pBackEndServices->GetAuthenticationClient()->m_PrincipalId != GUEST_PRINCIPAL_ID) {
        if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
            CompleteJob(m_ProtocolCallContext.m_Result);
            return;
        }
        bool isSuccess = false;
        if (m_LoginResult.Equals(isSuccess)) {
            CompleteJob(m_LoginResult);
            return;
        }
    }
    m_URL.SetRVConnectionID(m_pBackEndServices->m_pCredentials->m_ConnectionId);
    m_URL.SetPlatformType();
    if (m_pBackEndServices->m_UpdatesStationURLs) {
        qProtectedList<StationURL>& urls = Network::GetInstance()->m_StationUrls;
        ScopedCS lock(urls.m_Lock);
        for (qList<StationURL>::Node* pNode = urls.m_List.GetFirst(); !urls.m_List.IsEnd(pNode); pNode = pNode->m_pNext) {
            StationURL& url = pNode->m_Value;
            if ((url.GetType() & 2) == 0) {
                url.SetRVConnectionID(m_URL.GetRVConnectionID());
            }
            if (String::IsEqual(url.GetAddress().m_pString, m_URL.GetAddress().m_pString)) {
                url.SetType(m_URL.GetType());
                url.SetRVConnectionID(m_URL.GetRVConnectionID());
                m_URL.SetPortNumber(url.GetPortNumber());
            }
        }
        Network::GetInstance()->AddStationURL(m_URL);
    }
    m_pBackEndServices->m_URL = m_URL;
    m_pBackEndServices->RegisterServerProtocols();
    SetStep(Step(&JobBackEndServicesLogin::DisconnectFromAuthenticationService, ""));
}

// 0x003AFFA0 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::CompleteLogout()
{
    CompleteJob(m_Result);
}

// 0x003AFFC4 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ValidateArguments()
{
    if (m_Name.GetLength() == 0 || m_pPassword == nullptr) {
        CompleteJob(qResult(RESULT_INVALID_ARGUMENT));
        return;
    }
    SetStep(Step(&JobBackEndServicesLogin::ConnectToAuthenticationService, ""));
}

// 0x003B0050 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ProcessAuthConnectionResult()
{
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->ReleaseRef();
        m_pConnectJob = nullptr;
    }
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        if (m_pAuthenticationEndPoint != nullptr) {
            m_pAuthenticationEndPoint->Close();
            m_pAuthenticationEndPoint = nullptr;
        }
        CompleteJob(m_CallContext.m_Result);
        return;
    }
    m_pBackEndServices->m_pCredentials->m_pSecureConnection = m_pAuthenticationEndPoint;
    m_pBackEndServices->GetAuthenticationClient()->SetDefaultCredentials(m_pBackEndServices->m_pCredentials);
    SetStep(Step(&JobBackEndServicesLogin::LoginToAuthenticationService, ""));
}

// 0x003B0150 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ProcessAuthenticationResult()
{
    if (m_IsCancelled) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    bool isSuccess = true;
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS || !m_LoginResult.Equals(isSuccess)) {
        CompleteJob(m_LoginResult);
        return;
    }
    m_pBackEndServices->m_pCredentials->m_PrincipalId = m_pBackEndServices->GetAuthenticationClient()->m_PrincipalId;
    Credentials* pCredentials = m_pBackEndServices->m_pCredentials;
    SecureStream* pStream = GetSecureStream(pCredentials);
    pStream->m_pCredentials = pCredentials;
    pStream->m_pAuthenticationClient->SetDefaultCredentials(pCredentials);
    pStream->m_pSecureConnectionClient->SetDefaultCredentials(pStream->m_pCredentials);
    RVClientCore* pCore = RVClientCore::GetInstance();
    pCore->m_ServerTime = m_ConnectionData.m_currentUTCTime.ToEpochTime();
    pCore->m_LoginTime = Time::GetTime();
    SetStep(Step(&JobBackEndServicesLogin::ConnectToSecureConnectionService, ""));
}

// 0x003B0298 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ConnectToAuthenticationService()
{
    StringStream stream;
    stream << L"prudp:/address=" << m_Address << L";port=" << static_cast<int>(m_Port) << L";stream=" << static_cast<int>(SECURE_STREAM_TYPE)
           << L";sid=" << SERVER_SESSION_ID << L";type=" << SERVER_URL_TYPE;
    StationURL url(stream.m_pBuffer);
    StreamManager* pStreamManager = CreateStreamManager();
    if (pStreamManager != nullptr) {
        m_pBackEndServices->m_pCredentials->m_pStreamManager = pStreamManager;
        pStreamManager->SetCredentials(m_pBackEndServices->m_pCredentials);
        Credentials* pCredentials = m_pBackEndServices->m_pCredentials;
        pCredentials->m_Password = String(m_pPassword);
        m_pBackEndServices->m_pCredentials->m_Name = m_Name;
        m_CallContext.Reset();
        u32 timeout = Time::ConvertDeadlineToTimeout(m_Deadline);
        if (GetConnectionManager(m_pBackEndServices->m_pCredentials)->Connect(&m_CallContext, url, &m_pAuthenticationEndPoint, timeout, &m_pConnectJob)) {
            m_pConnectJob->AcquireRef();
            SetToSuspended();
            ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLogin::ProcessAuthConnectionResult, ""));
            return;
        }
    }
    CompleteJob(qResult(RESULT_CALL_FAILED));
}

// 0x003B04F8 (name is ours)
void nn::nex::JobBackEndServicesLogin::CompleteLogin()
{
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x003B0530 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ProcessSecConnConnectionResult()
{
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->ReleaseRef();
        m_pConnectJob = nullptr;
    }
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        if (m_pSecureEndPoint != nullptr) {
            m_pSecureEndPoint->Close();
            m_pSecureEndPoint = nullptr;
        }
        CompleteJob(m_CallContext.m_Result);
        return;
    }
    m_pBackEndServices->m_pCredentials->m_pConnection = m_pSecureEndPoint;
    m_pBackEndServices->GetSecureConnectionClient()->Bind(m_pBackEndServices->m_pCredentials);
    SetStep(Step(&JobBackEndServicesLogin::RegisterURLs, ""));
}

// 0x003B0638 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::ConnectToSecureConnectionService()
{
    if (m_pBackEndServices->GetAuthenticationClient()->m_PrincipalId == GUEST_PRINCIPAL_ID) {
        m_pBackEndServices->m_pCredentials->m_ConnectionId = 1;
        SetStep(Step(&JobBackEndServicesLogin::ProcessRegistrationResult, ""));
        return;
    }
    String text;
    StationURL url;
    if (m_ConnectionData.m_urlRegularProtocols != StationURL(L"")) {
        url = m_ConnectionData.m_urlRegularProtocols;
    } else {
        // the secure server is on the next port of the authentication server
        text.Format(L"prudps:/address=%ls;port=%d;stream=%d;sid=%d;PID=%d;CID=1;type=%d", m_Address.m_pString, m_Port + 1, SECURE_STREAM_TYPE, SERVER_SESSION_ID,
                    GUEST_PRINCIPAL_ID, SERVER_URL_TYPE);
        url = text;
    }
    m_CallContext.Reset();
    Time deadline = m_Deadline;
    u32 timeout = Time::ConvertDeadlineToTimeout(deadline);
    if (!GetConnectionManager(m_pBackEndServices->m_pCredentials)->Connect(&m_CallContext, url, &m_pSecureEndPoint, timeout, &m_pConnectJob)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    m_pConnectJob->AcquireRef();
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLogin::ProcessSecConnConnectionResult, ""));
}

// 0x003B083C | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLogin::DisconnectFromAuthenticationService()
{
    EndPoint* pEndPoint = m_pBackEndServices->m_pCredentials->m_pSecureConnection;
    m_pBackEndServices->m_pCredentials->m_pSecureConnection = m_pSecureEndPoint;
    m_pBackEndServices->GetAuthenticationClient()->SetDefaultCredentials(m_pBackEndServices->m_pCredentials);
    m_pAuthenticationEndPoint = nullptr;
    m_CallContext.Reset();
    m_CallContext.m_Deadline = m_Deadline;
    if (!GetConnectionManager(m_pBackEndServices->m_pCredentials)->Disconnect(&m_CallContext, pEndPoint)) {
        CompleteJob(qResult(RESULT_SUCCESS));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLogin::CompleteLogin, ""));
}

// 0x003B0940 slot 0x24 | fefates:bytes
void nn::nex::JobBackEndServicesLogin::CancelJob()
{
    // the reason of the fault of the end points (EndPoint::SignalFaultEvent)
    const unsigned int FAULT_CANCELLED = 6;
    if (m_IsCancelled) {
        return;
    }
    m_IsCancelled = true;
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->CancelJob();
        m_pConnectJob->ReleaseRef();
        m_pConnectJob = nullptr;
    }
    if (m_pAuthenticationEndPoint != nullptr) {
        m_pAuthenticationEndPoint->SignalFaultEvent(FAULT_CANCELLED, true);
    }
    if (m_pSecureEndPoint != nullptr) {
        m_pSecureEndPoint->SignalFaultEvent(FAULT_CANCELLED, true);
    }
}

// 0x003B0A04 | fefates:bytes-fuzzy [tier B]
nn::nex::JobBackEndServicesLogin::JobBackEndServicesLogin(unsigned int callContextId, nn::nex::BackEndServices* pBackEndServices, nn::nex::qResult* pResult,
                                                          const nn::nex::String& name, const char* pPassword, const wchar_t* pAddress, unsigned short port,
                                                          nn::nex::Credentials** ppCredentials, nn::nex::String* pServerName,
                                                          nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData, int timeout,
                                                          nn::nex::StreamManager* (*pCreateStreamManager)())
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(callContextId), m_Name(name), m_Address(pAddress), m_Port(port),
      m_ppCredentials(ppCredentials), m_pBackEndServices(pBackEndServices), m_pResult(pResult), m_pServerName(pServerName), m_pData(pData), m_pConnectJob(nullptr),
      m_pAuthenticationEndPoint(nullptr), m_pSecureEndPoint(nullptr), m_Deadline(Time::ConvertTimeoutToDeadline(timeout)), m_IsCancelled(false),
      m_pCreateStreamManager(pCreateStreamManager)
{
    m_TraceFlags = 4;
    m_pPassword = nullptr;
    if (pPassword != nullptr) {
        m_pPassword = NewArray<char>(strlen(pPassword) + 1);
        if (strlen(pPassword) == 0) {
            m_pPassword[0] = '\0';
        } else {
            strcpy(m_pPassword, pPassword);
        }
    }
    *m_ppCredentials = nullptr;
    SetStep(Step(&JobBackEndServicesLogin::ValidateArguments, ""));
}

// 0x003B0C9C slot 0x00
// 0x003B0BB8 (deleting dtor)
nn::nex::JobBackEndServicesLogin::~JobBackEndServicesLogin()
{
    if (m_pConnectJob != nullptr) {
        m_pConnectJob->ReleaseRef();
        m_pConnectJob = nullptr;
    }
    if (m_pPassword != nullptr) {
        MemoryManager::Free(reinterpret_cast<u32*>(m_pPassword) - 1);
    }
}

// 0x0072D108 | fefates:bytes-fuzzy [tier B]
nn::nex::StreamManager* nn::nex::JobBackEndServicesLogin::CreateStreamManager() const
{
    StreamManager* pStreamManager;
    if (m_pCreateStreamManager != nullptr) {
        pStreamManager = m_pCreateStreamManager();
    } else {
        Network* pNetwork = Network::GetInstance();
        ClientStreamManager* pClientStreamManager =
            new ClientStreamManager(static_cast<Stream::Type>(SECURE_STREAM_TYPE), pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr);
        if (pClientStreamManager != nullptr) {
            pClientStreamManager->m_pBroker = new ClientProtocolRequestBroker();
        }
        pStreamManager = pClientStreamManager;
    }
    if (!pStreamManager->Initialize(m_pBackEndServices->m_Port, m_pBackEndServices->m_StreamPort, m_pBackEndServices->m_StreamFlags)) {
        if (m_pCreateStreamManager == nullptr) {
            pStreamManager->Terminate();
            delete pStreamManager;
        }
        return nullptr;
    }
    SecureStream* pStream = new SecureStream();
    pStream->m_pStream = pStreamManager->m_pStream;
    pStreamManager->AssociateSecureStream(pStream, SecureStream::FilterIncomingConnection);
    return pStreamManager;
}

} // namespace nex
} // namespace nn
