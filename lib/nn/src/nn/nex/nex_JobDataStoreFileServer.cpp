#include "nn/nex/nex_JobDataStoreFileServer.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_RootCaReader.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x0095F620 (name is ours)
const char* JobDataStoreFileServer::s_RootCaNames[ROOT_CA_COUNT] = {"CACERT_PUBLIC_CA_5.der", "CACERT_PUBLIC_CA_6.der", "CACERT_PUBLIC_CA_7.der",
                                                                    "CACERT_PUBLIC_CA_8.der"};

namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_FAILURE = static_cast<int>(0x80010001);
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_TOO_MANY_RETRIES = static_cast<int>(0x80050019);
const int RESULT_HTTP_ERROR = static_cast<int>(0x8005001A);
const int RESULT_NOT_FOUND = static_cast<int>(0x80690004);
const int RESULT_NO_ROOT_CA = static_cast<int>(0x80690009);
// the HTTP status codes
const int STATUS_TEMPORARY_REDIRECT = 307;
const int STATUS_NOT_FOUND = 404;
const int STATUS_CONFLICT = 409;
const int STATUS_INTERNAL_SERVER_ERROR = 500;
// the largest error body that is kept
const u32 MAX_ERROR_BODY_SIZE = 10240;
// the requests of a redirect and of a retry
const u32 MAX_REDIRECT_COUNT = 5;
const u32 MAX_RETRY_COUNT = 3;

// the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    Core* pCore = Core::GetInstance();
    return (pCore != nullptr ? pCore->m_pScheduler : nullptr)->m_Lock;
}

// (inline) the status code is not an error
inline bool IsSuccessStatus(int statusCode)
{
    return static_cast<u32>(statusCode - 1) < 299;
}
} // namespace

// 0x0039C4D4 slot 0x34 | fefates:callseq
nn::nex::qResult nn::nex::JobDataStoreFileServer::Initialize(unsigned int callContextId, const nn::nex::String& url, nn::nex::HttpConnection::Method method,
                                                            nn::nex::HttpEventListener* pListener, unsigned int chunkSize)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_URL = url;
    m_Method = method;
    m_pListener = pListener;
    m_ChunkSize = chunkSize;
    RootCaReader* pReader = new RootCaReader();
    bool isRead = pReader->Read(m_RootCas, s_RootCaNames, ROOT_CA_COUNT);
    delete pReader;
    if (!isRead) {
        return qResult(RESULT_NO_ROOT_CA);
    }
    SetStep(Step(&JobDataStoreFileServer::StepHttp, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

// 0x0039C638 | fefates:bytes [tier B]
void nn::nex::JobDataStoreFileServer::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_CallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetToComplete();
}

// 0x0039C6B8 slot 0x38
// 0x00772274 (thunk)
void nn::nex::JobDataStoreFileServer::PrepareRequest(nn::nex::HttpConnection* pConnection)
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    for (u32 i = 0; i < ROOT_CA_COUNT; i++) {
        if (!pConnection->AddRootCa(m_RootCas[i].begin(), m_RootCas[i].size())) {
            CompleteJob(qResult(RESULT_FAILURE));
            return;
        }
    }
    // only the root CAs of the file server
    pConnection->m_UsesRootCas = false;
    pConnection->m_UsesClientCert = false;
    m_pListener->PrepareRequest(pConnection);
}

// 0x0039C79C slot 0x44
// 0x00772358 (thunk)
bool nn::nex::JobDataStoreFileServer::ProcessResponse(const unsigned char* pData, unsigned int size)
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return false;
    }
    if (IsSuccessStatus(m_StatusCode)) {
        if (m_Method == HttpConnection::METHOD_POST) {
            return true;
        }
        return m_pListener->ProcessResponse(pData, size);
    }
    // the body of an error is kept
    if (m_ErrorBody.size() + size > MAX_ERROR_BODY_SIZE) {
        m_IsTooLarge = true;
        m_CallContext.Cancel(CallContext::STATE_CANCELLED);
    } else {
        m_ErrorBody.insert(m_ErrorBody.end(), pData, pData + size);
    }
    return true;
}

// 0x0039C88C slot 0x3C
// 0x00772448 (thunk)
unsigned int nn::nex::JobDataStoreFileServer::PostChunkedBuffer(nn::nex::HttpConnection* pConnection, unsigned char* pBuffer, unsigned int size, bool* pIsLast)
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return 0;
    }
    return m_pListener->PostChunkedBuffer(pConnection, pBuffer, size, pIsLast);
}

// 0x0039C924 | fefates:bytes [tier B]
void nn::nex::JobDataStoreFileServer::StepWaitingForHttp()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_CallContext.m_State != CallContext::STATE_SUCCESS) {
        // (a message was logged: "HTTP Connection failure 0x%x")
        CompleteJob(m_CallContext.m_Result);
        return;
    }
    if (IsSuccessStatus(m_StatusCode)) {
        CompleteJob(qResult(RESULT_SUCCESS));
        return;
    }
    m_ErrorBody.push_back(0);
    if (m_IsTooLarge) {
        CompleteJob(qResult(RESULT_HTTP_ERROR));
        return;
    }
    switch (m_StatusCode) {
        case STATUS_TEMPORARY_REDIRECT:
            if (String::IsEqual(m_Location.m_pString, L"") || m_RedirectCount >= MAX_REDIRECT_COUNT) {
                CompleteJob(qResult(RESULT_HTTP_ERROR));
                return;
            }
            m_URL = m_Location;
            SetStep(Step(&JobDataStoreFileServer::StepHttp, ""));
            return;
        case STATUS_NOT_FOUND:
            CompleteJob(qResult(RESULT_NOT_FOUND));
            return;
        case STATUS_CONFLICT:
        case STATUS_INTERNAL_SERVER_ERROR:
            if (m_RedirectCount >= MAX_RETRY_COUNT) {
                CompleteJob(qResult(RESULT_TOO_MANY_RETRIES));
                return;
            }
            SetStep(Step(&JobDataStoreFileServer::StepHttp, ""));
            return;
        default:
            CompleteJob(qResult(RESULT_HTTP_ERROR));
            return;
    }
}

// 0x0039CC00 slot 0x40
// 0x007724E0 (thunk)
void nn::nex::JobDataStoreFileServer::ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode)
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_StatusCode = statusCode;
    if (statusCode == STATUS_TEMPORARY_REDIRECT) {
        pConnection->GetHeaderField("Location", &m_Location);
    }
    if (IsSuccessStatus(m_StatusCode)) {
        m_pListener->ProcessResponseHeader(pConnection, statusCode);
    }
}

// 0x0039CCBC | fefates:bytes [tier B]
void nn::nex::JobDataStoreFileServer::StepHttp()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_StatusCode = 0;
    m_ErrorBody.clear();
    m_IsTooLarge = false;
    m_Location = L"";
    m_RedirectCount++;
    m_CallContext.Reset();
    qResult result = m_HttpClient.Start(&m_CallContext, m_URL, static_cast<HttpConnection::Method>(m_Method), this, m_ChunkSize);
    if (!result) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    // the request is cancelled with the call of the caller
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_CallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_CallContext, new Step(&JobDataStoreFileServer::StepWaitingForHttp, ""));
}

// 0x0039CEB0 slot 0x00
// 0x0039CE40 (deleting dtor)
// 0x00772608 (thunk)
// 0x0077259C (deleting thunk)
nn::nex::JobDataStoreFileServer::~JobDataStoreFileServer()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
