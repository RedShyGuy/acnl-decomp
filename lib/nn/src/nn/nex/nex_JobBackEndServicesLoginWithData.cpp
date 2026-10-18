#include "nn/nex/nex_JobBackEndServicesLoginWithData.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_AuthenticationInfo.h"
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_ConnectionManager.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_JobConnectEndPoint.h"
#include "nn/nex/nex_JobLogin.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_SecureStream.h"
#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_TicketManager.h"

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

// 0x003C5C18 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::CompleteJob(nn::nex::qResult result)
{
    m_Result = result;
    if (!m_Result && m_pBackEndServices->m_pCredentials != nullptr) {
        m_CallContext.Reset();
        if (m_pBackEndServices->LogoutImpl(&m_CallContext, m_pBackEndServices->m_pCredentials)) {
            SetToSuspended();
            ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLoginWithData::CompleteLogout, ""));
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

// 0x003C5D68 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::Authenticate()
{
    if (GetSecureStream(m_pBackEndServices->m_pCredentials)->m_pCredentials != nullptr) {
        CompleteJob(qResult(RESULT_CONNECTION_FAILURE));
        return;
    }
    AuthenticationClient* pClient = m_pBackEndServices->GetAuthenticationClient();
    m_ProtocolCallContext.m_Deadline = m_Deadline;
    pClient->m_CallDeadline = m_ProtocolCallContext.m_Deadline;
    if (!pClient->m_pTicketManager->LoginWithData(&m_ProtocolCallContext, &m_LoginResult, m_pPassword, &pClient->m_PrincipalId, &m_ConnectionData, &m_Data)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobBackEndServicesLoginWithData::ProcessAuthenticationResult, ""));
}

// 0x003C5EAC | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::RegisterURLs()
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
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobBackEndServicesLoginWithData::ProcessRegistrationResult, ""));
}

// 0x003C5FF4 (name is ours)
void nn::nex::JobBackEndServicesLoginWithData::ProcessRegistrationResult()
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
    qProtectedList<StationURL>& urls = Network::GetInstance()->m_StationUrls;
    ScopedCS lock(urls.m_Lock);
    for (qList<StationURL>::Node* pNode = urls.m_List.GetFirst(); !urls.m_List.IsEnd(pNode); pNode = pNode->m_pNext) {
        if ((pNode->m_Value.GetType() & 2) == 0) {
            pNode->m_Value.SetRVConnectionID(m_URL.GetRVConnectionID());
        }
    }
    Network::GetInstance()->AddStationURL(m_URL);
    m_pBackEndServices->RegisterServerProtocols();
    SetStep(Step(&JobBackEndServicesLoginWithData::DisconnectFromAuthenticationService, ""));
}

// 0x003C6194 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::CompleteLogout()
{
    CompleteJob(m_Result);
}

// 0x003C61B8 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::ValidateArguments()
{
    if (static_cast<AuthenticationInfo*>(m_Data.m_pObject)->m_Token.GetLength() == 0 || m_pPassword == nullptr) {
        CompleteJob(qResult(RESULT_INVALID_ARGUMENT));
        return;
    }
    SetStep(Step(&JobBackEndServicesLoginWithData::ConnectToAuthenticationService, ""));
}

// 0x003C6248 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::ProcessAuthConnectionResult()
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
    SetStep(Step(&JobBackEndServicesLoginWithData::Authenticate, ""));
}

// 0x003C6348 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::ProcessAuthenticationResult()
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
    SetStep(Step(&JobBackEndServicesLoginWithData::ConnectToSecureConnectionService, ""));
}

// 0x003C6490 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::ConnectToAuthenticationService()
{
    StringStream stream;
    stream << L"prudp:/address=" << m_Address << L";port=" << static_cast<int>(m_Port) << L";stream=" << static_cast<int>(SECURE_STREAM_TYPE)
           << L";sid=" << SERVER_SESSION_ID << L";type=" << SERVER_URL_TYPE;
    StationURL url(stream.m_pBuffer);
    u32 timeout = Time::ConvertDeadlineToTimeout(m_Deadline);
    if (!GetConnectionManager(m_pBackEndServices->m_pCredentials)->Connect(&m_CallContext, url, &m_pAuthenticationEndPoint, timeout, &m_pConnectJob)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    m_pConnectJob->AcquireRef();
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLoginWithData::ProcessAuthConnectionResult, ""));
}

// 0x003C667C (name is ours)
void nn::nex::JobBackEndServicesLoginWithData::CompleteLogin()
{
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x003C66B4 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::ProcessSecConnConnectionResult()
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
    SetStep(Step(&JobBackEndServicesLoginWithData::RegisterURLs, ""));
}

// 0x003C67BC | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::ConnectToSecureConnectionService()
{
    if (m_pBackEndServices->GetAuthenticationClient()->m_PrincipalId == GUEST_PRINCIPAL_ID) {
        m_pBackEndServices->m_pCredentials->m_ConnectionId = 1;
        SetStep(Step(&JobBackEndServicesLoginWithData::ProcessRegistrationResult, ""));
        return;
    }
    String text;
    StationURL url;
    if (m_ConnectionData.m_urlRegularProtocols != StationURL(L"")) {
        url = m_ConnectionData.m_urlRegularProtocols;
    } else {
        // the secure server is on the next port of the authentication server
        text.Format(L"prudps:/address=%s;port=%d;stream=%d;sid=%d;PID=%d;CID=1;type=%d", m_Address.m_pString, m_Port + 1, SECURE_STREAM_TYPE, SERVER_SESSION_ID,
                    GUEST_PRINCIPAL_ID, SERVER_URL_TYPE);
        url = text;
    }
    // the custom data goes to the registration
    Data* pCustomData = m_pCustomData;
    m_pCustomData = nullptr;
    g_LoginCustomData.Reset(pCustomData);
    m_CallContext.Reset();
    Time deadline = m_Deadline;
    u32 timeout = Time::ConvertDeadlineToTimeout(deadline);
    if (!GetConnectionManager(m_pBackEndServices->m_pCredentials)->Connect(&m_CallContext, url, &m_pSecureEndPoint, timeout, &m_pConnectJob)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    m_pConnectJob->AcquireRef();
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLoginWithData::ProcessSecConnConnectionResult, ""));
}

// 0x003C6A04 | fefates:bytes [tier B]
void nn::nex::JobBackEndServicesLoginWithData::DisconnectFromAuthenticationService()
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
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobBackEndServicesLoginWithData::CompleteLogin, ""));
}

} // namespace nex
} // namespace nn
