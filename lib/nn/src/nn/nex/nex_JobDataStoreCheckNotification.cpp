#include "nn/nex/nex_JobDataStoreCheckNotification.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreFileServerClient.h"
#include "nn/nex/nex_DataStoreGetNotificationUrlParam.h"
#include "nn/nex/nex_DataStoreLogicServerClientInterface.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Time.h"

#include <cstdlib>

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
// the largest notification file
const u32 MAX_RESPONSE_SIZE = 128;
// the buffer of the request
const u32 REQUEST_BUFFER_SIZE = 0x4000;
// the separator of the numbers of the file
const char SEPARATOR = ',';
// the interval of a file without one
const u32 DEFAULT_INTERVAL = 1;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the call context of the caller
inline CallContext* GetCallerContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// 0x003BF818 slot 0x34 | fefates:callseq
nn::nex::qResult nn::nex::JobDataStoreCheckNotification::Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient,
                                                                   unsigned int threadPriority, int timeout, const nn::nex::String& url, nn::nex::String* pUrl,
                                                                   unsigned long long lastDataId, unsigned long long* pLastDataId, unsigned int interval,
                                                                   unsigned int* pInterval, unsigned int principalId, bool* pIsLogicServerError, bool getsUrl)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_pClient = pClient;
    m_pFileServerClient = new DataStoreFileServerClient();
    m_pUrl = pUrl;
    m_ThreadPriority = threadPriority;
    m_Timeout = timeout;
    m_Url = url;
    m_pLastDataId = pLastDataId;
    m_pInterval = pInterval;
    m_Interval = interval;
    m_PrincipalId = principalId;
    m_LastDataId = lastDataId;
    m_GetsUrl = getsUrl;
    m_pIsLogicServerError = pIsLogicServerError;
    *pIsLogicServerError = false;
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    if (m_GetsUrl || String::IsEqual(m_Url.m_pString, L"") || String::IsEqual(m_Url.m_pString, L"invalidurl") || m_Interval == 0) {
        SetStep(Step(&JobDataStoreCheckNotification::StepLogicServerGetNotificationUrl, ""));
    } else {
        SetStep(Step(&JobDataStoreCheckNotification::StepFileServerCheckNotificationFile, ""));
    }
    bool isSuccess = true;
    return qResult(isSuccess);
}

// 0x003BF9D0 | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            if (m_IsInvalid) {
                *m_pUrl = L"invalidurl";
                *m_pLastDataId = 0;
                *m_pInterval = 0;
            } else {
                *m_pUrl = m_Url;
                *m_pLastDataId = m_LastDataId;
                *m_pInterval = m_Interval;
            }
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetToComplete();
}

// 0x003BFAC4 slot 0x38
// 0x00772670 (thunk)
void nn::nex::JobDataStoreCheckNotification::PrepareRequest(nn::nex::HttpConnection* pConnection)
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    pConnection->m_ThreadPriority = m_ThreadPriority;
    if (m_UrlInfo.m_RootCaCert.size() != 0) {
        pConnection->AddRootCa(m_UrlInfo.m_RootCaCert.begin(), m_UrlInfo.m_RootCaCert.size());
    }
}

// 0x003BFB4C slot 0x40
// 0x007726F8 (thunk)
bool nn::nex::JobDataStoreCheckNotification::ProcessResponse(const unsigned char* pData, unsigned int size)
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return false;
    }
    if (m_Response.size() + size > MAX_RESPONSE_SIZE) {
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
        m_IsTooLarge = true;
        return false;
    }
    m_Response.insert(m_Response.end(), pData, pData + size);
    return true;
}

// 0x003BFBFC slot 0x3C
// 0x007727AC (thunk)
void nn::nex::JobDataStoreCheckNotification::ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode)
{
    m_StatusCode = statusCode;
}

// 0x003BFC04 (name is ours)
void nn::nex::JobDataStoreCheckNotification::StepLogicServerGetNotificationUrl()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    DataStoreGetNotificationUrlParam param;
    if (!m_IsTooLarge) {
        // (the server gets the URL of the last check)
        param.m_PreviousUrl = m_Url;
    }
    if (!m_pClient->GetNotificationUrl(&m_ProtocolCallContext, param, &m_UrlInfo)) {
        *m_pIsLogicServerError = true;
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobDataStoreCheckNotification::StepWaitingLogicServerGetNotificationUrl, ""));
}

// 0x003BFD94 | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::StepFileServerCheckNotificationFile()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    m_ProtocolCallContext.m_Deadline = Time::ConvertTimeoutToDeadline(m_Timeout);
    qResult result = m_pFileServerClient->ApiImpl(&m_ProtocolCallContext, m_Url, this, HttpConnection::METHOD_GET, REQUEST_BUFFER_SIZE);
    if (!result) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobDataStoreCheckNotification::StepWaitingFileServerCheckNotificationFile, ""));
}

// 0x003BFECC | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::StepWaitingLogicServerGetNotificationUrl()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        *m_pIsLogicServerError = true;
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    m_Url = m_UrlInfo.m_Url + m_UrlInfo.m_Key + m_UrlInfo.m_Query;
    bool hasScheme = m_Url.FindSubstringNoCase(L"http://") == 0 || m_Url.FindSubstringNoCase(L"https://") == 0;
    m_Url = hasScheme ? m_Url + L"" : L"https://" + m_Url;
    if (m_Interval == 0) {
        m_Interval = DEFAULT_INTERVAL;
    }
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x003C006C | fefates:bytes [tier B]
void nn::nex::JobDataStoreCheckNotification::StepWaitingFileServerCheckNotificationFile()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        qResult result = m_ProtocolCallContext.m_Result;
        if (m_IsTooLarge) {
            // not a notification file: a new URL
            SetStep(Step(&JobDataStoreCheckNotification::StepLogicServerGetNotificationUrl, ""));
            return;
        }
        CompleteJob(result);
        return;
    }
    u32 interval = m_Interval;
    u32 principalId = m_PrincipalId;
    m_Response.push_back(0);
    char* pEnd = nullptr;
    u64 dataId = strtoull(reinterpret_cast<const char*>(m_Response.begin()), &pEnd, 10);
    if (*pEnd == SEPARATOR) {
        u32 filePrincipalId = strtoul(pEnd + 1, &pEnd, 10);
        if (principalId != 0 && *pEnd == SEPARATOR && filePrincipalId == principalId) {
            u32 fileInterval = strtoul(pEnd + 1, &pEnd, 10);
            if ((fileInterval != 0 && interval == fileInterval) || interval == 1 || interval == 0) {
                m_Interval = fileInterval;
                if (m_LastDataId < dataId) {
                    m_LastDataId = dataId;
                }
                CompleteJob(qResult(RESULT_SUCCESS));
                return;
            }
        }
    }
    m_IsInvalid = true;
    CompleteJob(qResult(RESULT_SUCCESS));
}

// 0x003C028C | fefates:bytes [tier B]
nn::nex::JobDataStoreCheckNotification::JobDataStoreCheckNotification()
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pClient(nullptr), m_pFileServerClient(nullptr), m_pUrl(nullptr), m_Url(L""),
      m_ThreadPriority(0), m_Timeout(0), m_pLastDataId(nullptr), m_LastDataId(0), m_pInterval(nullptr), m_Interval(0), m_PrincipalId(0), m_StatusCode(0), m_IsTooLarge(false),
      m_GetsUrl(false), m_pIsLogicServerError(nullptr), m_IsInvalid(false)
{
    // nothing to do
}

// 0x003C038C slot 0x00 | fefates:bytes
// 0x003C0374 (deleting dtor)
// 0x003C0384 (thunk)
// 0x007727B4 (deleting thunk)
nn::nex::JobDataStoreCheckNotification::~JobDataStoreCheckNotification()
{
    if (m_pFileServerClient != nullptr) {
        delete m_pFileServerClient;
    }
}

} // namespace nex
} // namespace nn
