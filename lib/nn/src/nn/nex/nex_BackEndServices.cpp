#include <cwchar>
#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_AuthenticationClient.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_GlobalNotificationEventManager.h"
#include "nn/nex/nex_JobBackEndServicesLogin.h"
#include "nn/nex/nex_JobBackEndServicesLogout.h"
#include "nn/nex/nex_JobBackEndServicesTerminate.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_NintendoNotificationEventManager.h"
#include "nn/nex/nex_NotificationEventManager.h"
#include "nn/nex/nex_RVClientCore.h"
#include "nn/nex/nex_RendezVousLoginOperation.h"
#include "nn/nex/nex_RendezVousLogoutOperation.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SecureConnectionClient.h"
#include "nn/nex/nex_SecureStream.h"
#include "nn/nex/nex_StreamManager.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// 0x00AE05D0 (name is ours)
wchar_t BackEndServices::s_SandboxAccessKey[128];

namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
// the back end is not connected
const int RESULT_NOT_CONNECTED = static_cast<int>(0x800300D2);
// the timeout of the login (ms)
const s32 LOGIN_TIMEOUT = 30000;
// the protocol ids of the notification event managers
const u16 NOTIFICATION_PROTOCOL_ID = 14;
const u16 NINTENDO_NOTIFICATION_PROTOCOL_ID = 100;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the job is queued
inline void QueueJob(Job* pJob)
{
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
}

// (inline) a completed job is released; false if it is still in progress
inline bool ReleaseCompletedJob(Job*& pJob)
{
    if (pJob != nullptr) {
        if (pJob->m_State != Job::STATE_COMPLETED) {
            return false;
        }
        pJob->ReleaseRef();
        pJob = nullptr;
    }
    return true;
}

// (inline) the job is in progress
inline bool IsInProgress(const Job* pJob)
{
    return pJob != nullptr && pJob->m_State != Job::STATE_COMPLETED;
}

// (inline) the secure stream of the credentials
inline SecureStream* GetSecureStream(const Credentials* pCredentials)
{
    if (pCredentials == nullptr || pCredentials->m_pStreamManager == nullptr) {
        return nullptr;
    }
    return static_cast<SecureStream*>(pCredentials->m_pStreamManager->m_pSecureStream);
}

// the names of the errors of the PythonCore facility
// 0x003769F8 (name is ours)
DECOMP_NOINLINE String GetPythonCoreErrorName(u32 code)
{
    String name;
    switch (code) {
        case 1: name = L"Exception"; break;
        case 2: name = L"TypeError"; break;
        case 3: name = L"IndexError"; break;
        case 4: name = L"InvalidReference"; break;
        case 5: name = L"CallFailure"; break;
        case 6: name = L"MemoryError"; break;
        case 7: name = L"KeyError"; break;
        case 8: name = L"OperationError"; break;
        case 9: name = L"ConversionError"; break;
        case 10: name = L"ValidationError"; break;
    }
    return name;
}
} // namespace

namespace {
// the names of the errors of the RendezVous facility
// 0x00376ADC (name is ours)
DECOMP_NOINLINE String GetRendezVousErrorName(u32 code)
{
    String name;
    switch (code) {
        case 1: name = L"ConnectionFailure"; break;
        case 2: name = L"NotAuthenticated"; break;
        case 100: name = L"InvalidUsername"; break;
        case 101: name = L"InvalidPassword"; break;
        case 102: name = L"UsernameAlreadyExists"; break;
        case 103: name = L"AccountDisabled"; break;
        case 104: name = L"AccountExpired"; break;
        case 105: name = L"ConcurrentLoginDenied"; break;
        case 106: name = L"EncryptionFailure"; break;
        case 107: name = L"InvalidPID"; break;
        case 111: name = L"InvalidOperationInLiveEnvironment"; break;
    }
    return name;
}
} // namespace

namespace {
// the completion callback of the login and logout calls (the context is the operation; what it did
// was removed from this build, only the lock is left)
// 0x0052A2F4 (name is ours)
void OnOperationCompletion(CallContext* pContext, const UserContext* pUserContext)
{
    ScopedCS lock(GetSchedulerLock());
}
} // namespace

namespace {
// the completion callback of the pre-terminate call (the context is the back end services)
// 0x00376434 (name is ours)
void OnPreTerminateCompletion(CallContext* pContext, const UserContext* pUserContext)
{
    static_cast<BackEndServices*>(pUserContext->m_pPointer)->TerminateImpl();
}
} // namespace

// 0x0036E674
void nn::nex::BackEndServices::SetSandboxAccessKey(const nn::nex::String& key)
{
    wcsncpy(s_SandboxAccessKey, key.m_pString, 127);
}

// 0x00376098 | fefates:bytes [tier B]
bool nn::nex::BackEndServices::LogoutImpl(nn::nex::CallContext* pContext, nn::nex::Credentials* pCredentials)
{
    if (!ReleaseCompletedJob(m_pLogoutJob)) {
        return false;
    }
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (m_pCredentials == nullptr) {
        pContext->SetStateImpl(CallContext::STATE_SUCCESS, qResult(RESULT_SUCCESS), true);
        return true;
    }
    m_pLogoutJob = new JobBackEndServicesLogout(pContext->m_Id, this, pCredentials);
    m_pLogoutJob->AcquireRef();
    QueueJob(m_pLogoutJob);
    return true;
}

// 0x0037626C | fefates:callgraph [tier C]
void nn::nex::BackEndServices::OnLoginStart(nn::nex::CallContext& context)
{
    if (m_pLoginOperation == nullptr) {
        ScopedCS lock(GetSchedulerLock());
        if (m_pLoginOperation == nullptr) {
            m_pLoginOperation = new RendezVousLoginOperation(this);
        }
    }
    UserContext userContext;
    userContext.m_pPointer = m_pLoginOperation;
    context.RegisterCompletionCallback(OnOperationCompletion, userContext, false);
    // (the start of the operation was removed from this build)
    ScopedCS lock(GetSchedulerLock());
}

// 0x00376350 | fefates:callgraph [tier C]
void nn::nex::BackEndServices::OnLogoutStart(nn::nex::CallContext& context)
{
    if (m_pLogoutOperation == nullptr) {
        ScopedCS lock(GetSchedulerLock());
        if (m_pLogoutOperation == nullptr) {
            m_pLogoutOperation = new RendezVousLogoutOperation(this);
        }
    }
    UserContext userContext;
    userContext.m_pPointer = m_pLogoutOperation;
    context.RegisterCompletionCallback(OnOperationCompletion, userContext, false);
    // (the start of the operation was removed from this build)
    ScopedCS lock(GetSchedulerLock());
}

// 0x0037643C | fefates:callgraph [tier C]
bool nn::nex::BackEndServices::TerminateImpl()
{
    ScopedCS lock(GetSchedulerLock());
    if (!ReleaseCompletedJob(m_pTerminateJob)) {
        return false;
    }
    m_pTerminateJob = new JobBackEndServicesTerminate(this, m_pTerminateContext->m_Id);
    m_pTerminateJob->AcquireRef();
    QueueJob(m_pTerminateJob);
    return true;
}

// 0x003765D4 slot 0x08 | fefates:bytes
void nn::nex::BackEndServices::PostLogoutCleanup()
{
    if (m_pCredentials != nullptr) {
        m_pCredentials->ReleaseRef();
    }
    m_pCredentials = nullptr;
}

// 0x00376634
nn::nex::String nn::nex::BackEndServices::GetResultString(const nn::nex::String& name, nn::nex::qResult result)
{
    u32 code = result.m_Code;
    u32 facility = (code >> 16) & 0xFF;
    u32 error = code & 0xFFFF;
    String facilityName;
    String errorName;
    switch (facility) {
        case 1: {
            facilityName = L"Core";
            String text;
            switch (error) {
                case 1: text = L"Unknown"; break;
                case 4: text = L"OperationAborted"; break;
                case 5: text = L"Exception"; break;
                case 6: text = L"AccessDenied"; break;
                case 10: text = L"InvalidArgument"; break;
                case 11: text = L"Timeout"; break;
                case 12: text = L"InitializationFailure"; break;
            }
            errorName = text;
            break;
        }
        case 3:
            facilityName = L"RendezVous";
            errorName = GetRendezVousErrorName(error);
            break;
        case 4:
            facilityName = L"PythonCore";
            errorName = GetPythonCoreErrorName(error);
            break;
        case 5: {
            facilityName = L"Transport";
            String text;
            if (error == 2) {
                text = L"ConnectionFailure";
            }
            errorName = text;
            break;
        }
    }
    StringStream stream;
    if (facilityName.GetLength() != 0 && errorName.GetLength() != 0) {
        stream << name;
        stream << L" - (Facility:";
        stream << facilityName;
        stream << L", ErrorCode:";
        stream << errorName;
        stream << L") = ";
        hex(stream);
        stream.StreamNumber(code);
    } else {
        stream << name;
        stream << L" - ";
        hex(stream);
        stream.StreamNumber(code);
    }
    return String(stream.m_pBuffer);
}

// 0x00376964 slot 0x0C | fefates:bytes
void nn::nex::BackEndServices::RegisterServerProtocols()
{
    m_pNotificationEventManager->m_PrincipalId = m_pCredentials->m_PrincipalId;
    m_pNotificationEventManager->m_ConnectionId = m_pCredentials->m_ConnectionId;
    RVClientCore::GetInstance()->m_pNotificationManager->RegisterGlobalNotificationEventManager(m_pNotificationEventManager);
    m_pCredentials->m_pStreamManager->m_pBroker->RegisterServerProtocol(NINTENDO_NOTIFICATION_PROTOCOL_ID, m_pNintendoNotificationEventManager);
}

// 0x003769C0 slot 0x10 | fefates:bytes
void nn::nex::BackEndServices::UnregisterServerProtocols()
{
    RVClientCore::GetInstance()->m_pNotificationManager->UnregisterGlobalNotificationEventManager(m_pNotificationEventManager);
    m_pCredentials->m_pStreamManager->m_pBroker->UnregisterServerProtocol(NINTENDO_NOTIFICATION_PROTOCOL_ID);
}

// 0x00376C00 | mk7dlp:bytes [tier A]
void nn::nex::BackEndServices::RegisterPreTerminateCallback(void (*pFunction)(nn::nex::CallContext*, const nn::nex::UserContext*), const nn::nex::UserContext& context)
{
    m_PreTerminateCallback.m_pFunction = pFunction;
    m_PreTerminateCallback.m_Context = context;
    m_PreTerminateCallback.m_IsSet = true;
}

// 0x00376C78 | fefates:callgraph [tier C]
bool nn::nex::BackEndServices::Logout_(nn::nex::CallContext* pContext, nn::nex::Credentials* pCredentials)
{
    ScopedCS lock(GetSchedulerLock());
    if (!ReleaseCompletedJob(m_pTerminateJob)) {
        return false;
    }
    if (!ReleaseCompletedJob(m_pLoginJob)) {
        return false;
    }
    return LogoutImpl(pContext, pCredentials);
}

// 0x00376DA8 | fefates:callgraph [tier C]
bool nn::nex::BackEndServices::LoginImpl(nn::nex::CallContext* pContext, nn::nex::qResult* pResult, const wchar_t* pName, const char* pPassword, const wchar_t* pAddress,
                                         unsigned short port, nn::nex::Credentials** ppCredentials, nn::nex::String* pServerName,
                                         nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>* pData)
{
    Core::GetInstance()->SetInterrupted(false);
    ScopedCS lock(GetSchedulerLock());
    if (!ReleaseCompletedJob(m_pTerminateJob)) {
        return false;
    }
    if (!ReleaseCompletedJob(m_pLoginJob)) {
        return false;
    }
    if (!ReleaseCompletedJob(m_pLogoutJob)) {
        return false;
    }
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (m_pCredentials == nullptr) {
        m_pCredentials = new Credentials();
    }
    if (m_LoginJobTimeout == 0) {
        m_LoginJobTimeout = m_LoginTimeout;
    }
    m_pLoginJob = new JobBackEndServicesLogin(pContext->m_Id, this, pResult, String(pName), pPassword, pAddress, port, ppCredentials, pServerName, pData, m_LoginJobTimeout,
                                              m_pCreateStreamManager);
    m_pLoginJob->AcquireRef();
    QueueJob(m_pLoginJob);
    return true;
}

// 0x0037708C | fefates:bytes [tier B]
bool nn::nex::BackEndServices::Terminate(nn::nex::CallContext* pContext)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    m_pTerminateContext = pContext;
    if (!m_PreTerminateCallback.m_IsSet) {
        return TerminateImpl();
    }
    m_PreTerminateCallContext.Reset();
    UserContext context;
    context.m_pPointer = this;
    m_PreTerminateCallContext.RegisterCompletionCallback(OnPreTerminateCompletion, context, true);
    m_PreTerminateCallback.m_pFunction(&m_PreTerminateCallContext, &m_PreTerminateCallback.m_Context);
    return true;
}

// 0x00377100
nn::nex::BackEndServices::BackEndServices()
    : m_LoginTimeout(LOGIN_TIMEOUT), m_pLoginJob(nullptr), m_pLogoutJob(nullptr), m_pTerminateJob(nullptr), m_LoginJobTimeout(0), m_pLoginOperation(nullptr),
      m_pLogoutOperation(nullptr), m_UpdatesStationURLs(true)
{
    m_pCredentials = nullptr;
    Core::AcquireInstance();
    Network::AcquireInstance();
    RVClientCore::CreateInstance();
    m_Port = RootTransport::GetWellKnownPortNumber();
    m_StreamPort = 0;
    m_StreamFlags = 1;
    m_Unknown0x8C = 0;
    m_pNotificationEventManager = nullptr;
    m_pNintendoNotificationEventManager = nullptr;
    m_pCreateStreamManager = nullptr;
    m_pLoginJob = nullptr;
    m_pLogoutJob = nullptr;
    m_pTerminateJob = nullptr;
    m_pTerminateContext = nullptr;
    m_pNotificationEventManager = new NotificationEventManager();
    m_pNotificationEventManager->m_ProtocolId = NOTIFICATION_PROTOCOL_ID;
    m_pNotificationEventManager->SetFlag(Protocol::FLAG_UNKNOWN_8);
    m_pNintendoNotificationEventManager = new NintendoNotificationEventManager(nullptr);
    m_pNintendoNotificationEventManager->m_ProtocolId = NINTENDO_NOTIFICATION_PROTOCOL_ID;
    m_pNintendoNotificationEventManager->SetFlag(Protocol::FLAG_UNKNOWN_8);
}

// 0x00377288 slot 0x00 | fefates:callseq
// 0x00377274 (deleting dtor)
nn::nex::BackEndServices::~BackEndServices()
{
    if (m_pCredentials != nullptr) {
        CallContext context;
        Terminate(&context);
        context.Wait(0xFFFFFFFF);
    }
    if (m_pTerminateJob != nullptr) {
        m_pTerminateJob->ReleaseRef();
    }
    if (m_pLoginJob != nullptr) {
        m_pLoginJob->ReleaseRef();
    }
    if (m_pLogoutJob != nullptr) {
        m_pLogoutJob->ReleaseRef();
        m_pLogoutJob = nullptr;
    }
    delete m_pNotificationEventManager;
    m_pNotificationEventManager = nullptr;
    delete m_pNintendoNotificationEventManager;
    m_pNintendoNotificationEventManager = nullptr;
    m_pTerminateContext = nullptr;
    if (m_pLoginOperation != nullptr) {
        delete m_pLoginOperation;
        m_pLoginOperation = nullptr;
    }
    if (m_pLogoutOperation != nullptr) {
        delete m_pLogoutOperation;
        m_pLogoutOperation = nullptr;
    }
    RVClientCore::ReleaseInstance();
    Network::ReleaseInstance();
    Core::ReleaseInstance();
}

// 0x0072B468
nn::nex::qResult nn::nex::BackEndServices::IsConnected() const
{
    ScopedCS lock(GetSchedulerLock());
    if (IsInProgress(m_pLogoutJob) || m_pLoginJob == nullptr || m_pLoginJob->m_State != Job::STATE_COMPLETED) {
        return qResult(RESULT_NOT_CONNECTED);
    }
    SecureConnectionClient* pClient = GetSecureConnectionClient();
    if (pClient == nullptr || !pClient->IsConnected() || pClient->IsFaulty() || pClient->m_pDefaultCredentials->m_PrincipalId == 0) {
        return qResult(RESULT_NOT_CONNECTED);
    }
    Network* pNetwork = Network::GetInstance();
    if ((pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr)->m_Unknown0xFE) {
        return qResult(RESULT_NOT_CONNECTED);
    }
    return qResult(RESULT_SUCCESS);
}

// 0x0072B5F8 | fefates:bytes [tier B]
bool nn::nex::BackEndServices::LoginJobIsInProgress() const
{
    return IsInProgress(m_pLoginJob);
}

// 0x0072B614 | fefates:bytes [tier B]
nn::nex::AuthenticationClient* nn::nex::BackEndServices::GetAuthenticationClient() const
{
    SecureStream* pStream = GetSecureStream(m_pCredentials);
    AuthenticationClient* pClient = pStream != nullptr ? pStream->m_pAuthenticationClient : nullptr;
    if (pClient != nullptr) {
        pClient->SetDefaultCredentials(m_pCredentials);
    }
    return pClient;
}

// 0x0072B660 | fefates:bytes [tier B]
bool nn::nex::BackEndServices::TerminateJobIsInProgress() const
{
    return IsInProgress(m_pTerminateJob);
}

// 0x0072B67C | fefates:bytes [tier B]
nn::nex::SecureConnectionClient* nn::nex::BackEndServices::GetSecureConnectionClient() const
{
    SecureStream* pStream = GetSecureStream(m_pCredentials);
    SecureConnectionClient* pClient = pStream != nullptr ? pStream->m_pSecureConnectionClient : nullptr;
    if (pClient != nullptr) {
        pClient->SetDefaultCredentials(m_pCredentials);
    }
    return pClient;
}

// 0x00376C20 (name is ours)
bool nn::nex::BackEndServices::RegisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler)
{
    if (m_pNotificationEventManager == nullptr) {
        return false;
    }
    return m_pNotificationEventManager->RegisterEventHandler(pHandler);
}

// 0x00376C3C (name is ours)
bool nn::nex::BackEndServices::UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler* pHandler)
{
    if (m_pNotificationEventManager == nullptr) {
        return false;
    }
    return m_pNotificationEventManager->UnregisterEventHandler(pHandler);
}

} // namespace nex
} // namespace nn
