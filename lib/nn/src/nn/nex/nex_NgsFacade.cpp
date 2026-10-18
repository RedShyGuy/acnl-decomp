#include "nn/nex/nex_NgsFacade.h"
#include "nn/friends/CTR/detail/detail_Api.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_ErrorCodeConverter.h"
#include "nn/nex/nex_JobCTRLogin.h"
#include "nn/nex/nex_JobTerminate.h"
#include "nn/nex/nex_NATProperties.h"
#include "nn/nex/nex_NgsBridgeImp.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_RootTransport.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x0096B0F4 (name is ours)
NgsFacade::LoginErrorCodes NgsFacade::s_LoginErrorCodes = {20000, 23000, 0x0FE00300, 0x0FE003FF};

namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_UNSUPPORTED = static_cast<int>(0x80010002);
const int RESULT_IN_PROGRESS = static_cast<int>(0x80010004);
const int RESULT_INVALID_ARGUMENT = static_cast<int>(0x8001000A);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_NOT_INITIALIZED = static_cast<int>(0x80650000);
const int RESULT_NOT_LOGGED_IN = static_cast<int>(0x80650002);
const int RESULT_OTHER_PRINCIPAL = static_cast<int>(0x80650009);
// the error of the friends server (GetLastLoginErrorCode)
const int RESULT_FRIENDS_ERROR = static_cast<int>(0x80680001);

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

// (inline) the job is in progress
inline bool IsInProgress(const Job* pJob)
{
    return pJob != nullptr && pJob->m_State != Job::STATE_COMPLETED;
}

// (inline) the job is completed and released
template <typename T>
inline void CompleteAndRelease(T*& pJob)
{
    if (pJob != nullptr) {
        if (pJob->m_State != Job::STATE_COMPLETED) {
            pJob->SetToComplete();
        }
        pJob->ReleaseRef();
        pJob = nullptr;
    }
}
} // namespace

// 0x003D6B28
void nn::nex::NgsFacade::OnPreTerminate(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext)
{
    static_cast<NgsFacade*>(pUserContext->m_pPointer)->StartTerminate(pContext);
}

// 0x003D6B38
void nn::nex::NgsFacade::StartTerminate(nn::nex::CallContext* pContext)
{
    ScopedCS lock(GetSchedulerLock());
    if (!pContext->InitiateCall()) {
        return;
    }
    if (m_pTerminateJob != nullptr) {
        if (m_pTerminateJob->m_State != Job::STATE_COMPLETED) {
            return;
        }
        m_pTerminateJob->ReleaseRef();
        m_pTerminateJob = nullptr;
    }
    m_pTerminateJob = new JobTerminate(this, pContext->m_Id);
    m_pTerminateJob->AcquireRef();
    QueueJob(m_pTerminateJob);
}

// 0x003D6CC4 | fefates:bytes [tier B]
nn::nex::qResult nn::nex::NgsFacade::LoginAndRequestAuthenticationTokenImpl(nn::nex::ProtocolCallContext* pContext, unsigned int gameServerId, const wchar_t* pAccessKey,
                                                                           const char* pKeyHash, char* pServiceToken, const void* pUnsupported, int timeout,
                                                                           unsigned int principalId)
{
    m_LastHttpStatusCode = 200;
    m_LastResponseResult = 0;
    m_Unknown0x13C = 0;
    m_LoginResult = qResult(RESULT_SUCCESS);
    if (pContext == nullptr || pAccessKey == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    if (pKeyHash != nullptr && pServiceToken == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    if (gameServerId >= s_LoginErrorCodes.m_SampleServerIdFirst && s_LoginErrorCodes.m_SampleServerIdLast >= gameServerId) {
        // (a message was logged: "0x%08x is the game server ID for NEX sample demo server.")
    }
    if (pUnsupported != nullptr) {
        return qResult(RESULT_UNSUPPORTED);
    }
    if (!friends::CTR::detail::IsInitialized()) {
        return qResult(RESULT_NOT_INITIALIZED);
    }
    if (!m_Unknown0x141 && !friends::CTR::detail::HasLoggedIn()) {
        return qResult(RESULT_NOT_LOGGED_IN);
    }
    if (principalId != 0 && friends::CTR::detail::GetMyPrincipalId() != principalId) {
        return qResult(RESULT_OTHER_PRINCIPAL);
    }
    if (m_pLoginJob != nullptr) {
        if (m_pLoginJob->m_State != Job::STATE_COMPLETED) {
            return qResult(RESULT_IN_PROGRESS);
        }
        m_pLoginJob->ReleaseRef();
        m_pLoginJob = nullptr;
    }
    if (LoginJobIsInProgress() || m_pCredentials != nullptr || IsInProgress(m_pTerminateJob) || TerminateJobIsInProgress()) {
        return qResult(RESULT_IN_PROGRESS);
    }
    u16 port = RootTransport::GetNextPortNumber();
    RootTransport::GetInstance()->SetWellKnownPortNumber(port);
    m_Port = port;
    NATProperties properties;
    RootTransport::GetInstance()->SetNATProperties(properties);
    SetSandboxAccessKey(String(pAccessKey));
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    JobCTRLogin* pJob = new JobCTRLogin(DebugString());
    m_pLoginJob = pJob;
    pJob->m_CallContextId = pContext->m_Id;
    pJob->m_Unknown0xE4 = gameServerId;
    pJob->m_pBackEndServices = this;
    pJob->m_pKeyHash = pKeyHash;
    pJob->m_pServiceToken = pServiceToken;
    pJob->m_Timeout = timeout;
    pJob->m_StartTime = Time::GetTime();
    pJob->SetStep(StepSequenceJob::Step(&JobLogin::StepFirst, ""));
    m_pLoginJob->AcquireRef();
    QueueJob(m_pLoginJob);
    return qResult(RESULT_SUCCESS);
}

// 0x003D7164 | fefates:bytes [tier B]
nn::nex::qResult nn::nex::NgsFacade::Login(nn::nex::ProtocolCallContext* pContext, unsigned int gameServerId, const wchar_t* pAccessKey, int timeout, unsigned int principalId)
{
    return LoginAndRequestAuthenticationTokenImpl(pContext, gameServerId, pAccessKey, nullptr, nullptr, nullptr, timeout, principalId);
}

// 0x003D719C | fefates:bytes [tier B]
nn::nex::qResult nn::nex::NgsFacade::Logout(nn::nex::ProtocolCallContext* pContext)
{
    if (pContext == nullptr) {
        return qResult(RESULT_INVALID_ARGUMENT);
    }
    if (IsInProgress(m_pLoginJob)) {
        return qResult(RESULT_IN_PROGRESS);
    }
    if (!RendezVous::Logout(pContext, m_pCredentials)) {
        return qResult(RESULT_CALL_FAILED);
    }
    return qResult(RESULT_SUCCESS);
}

// 0x003D725C | fefates:bytes-fuzzy [tier B]
nn::nex::NgsFacade::NgsFacade()
    : m_Unknown0x118(0), m_pLoginJob(nullptr), m_pTerminateJob(nullptr), m_LastResponseResult(0), m_LastHttpStatusCode(200), m_LoginResult(RESULT_SUCCESS),
      m_Unknown0x13C(0), m_Unknown0x140(false), m_Unknown0x141(false), m_Unknown0x142(false)
{
    RootTransport::GetInstance()->m_UPnPProperties.m_Unknown0x09 = false;
    UserContext context;
    context.m_pPointer = this;
    RegisterPreTerminateCallback(OnPreTerminate, context);
    m_AuthenticationInfo.Reset(new AuthenticationInfo());
    m_pNgsBridge = new NgsBridgeImp(this);
}

// 0x003D73D0 slot 0x00 | fefates:bytes
// 0x003D73C0 (deleting dtor)
nn::nex::NgsFacade::~NgsFacade()
{
    if (m_pCredentials != nullptr) {
        CallContext context;
        Terminate(&context);
        context.Wait(0xFFFFFFFF);
    }
    delete m_AuthenticationInfo.Release();
    CompleteAndRelease(m_pLoginJob);
    CompleteAndRelease(m_pTerminateJob);
    delete m_pNgsBridge;
}

// 0x0072EA08 | fefates:bytes [tier B]
u32 nn::nex::NgsFacade::GetLastLoginErrorCode() const
{
    if (m_LoginResult.m_Code != qResult(RESULT_FRIENDS_ERROR).m_Code) {
        return ErrorCodeConverter::ConvertToNetworkErrorCode(m_LoginResult);
    }
    if (m_Unknown0x13C != 0) {
        return m_Unknown0x13C;
    }
    if (m_LastHttpStatusCode == 200) {
        return m_LastResponseResult + s_LoginErrorCodes.m_ResponseResultBase;
    }
    return m_LastHttpStatusCode + s_LoginErrorCodes.m_HttpStatusCodeBase;
}

} // namespace nex
} // namespace nn
