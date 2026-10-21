#include "nn/nex/nex_JobHttp.h"
#include "nn/nex/nex_CTRHttpConnection.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_HttpEventListener.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// 0x0096C9F8 (name is ours)
u32 JobHttp::s_ConnectionCount;
// 0x0096C9FC (name is ours)
CriticalSection* JobHttp::s_pConnectionLock;

namespace {
// the results (the names are ours)
const int RESULT_NO_CALL_CONTEXT = static_cast<int>(0x80010004);
const int RESULT_NOT_YET = static_cast<int>(0x80010001);
const int RESULT_CONNECTION_FAILED = static_cast<int>(0x80050002);
const int RESULT_NO_STATUS = static_cast<int>(0x8005000B);
// the listener did not take the data yet
const int RESULT_WAITING = static_cast<int>(0x80050016);
// the post data parts of one step
const int MAX_POST_PARTS = 100;

// the call context of the job
inline CallContext* GetCallContext(u32 id)
{
    return Core::GetInstance()->m_pCallContextRegister->GetCallContext(id);
}
} // namespace

// 0x003D227C slot 0x00 | fefates:bytes-fuzzy
// 0x003D224C (deleting dtor)
nn::nex::JobHttp::~JobHttp()
{
    if (m_pConnection != nullptr) {
        delete m_pConnection;
        s_ConnectionCount--;
        if (s_pConnectionLock != nullptr) {
            s_pConnectionLock->Leave();
        }
        m_pConnection = nullptr;
    }
}

// 0x003D1860 slot 0x34 | fefates:callseq
nn::nex::qResult nn::nex::JobHttp::Initialize(unsigned int callContextId, const nn::nex::String& url, nn::nex::HttpConnection::Method method,
                                              nn::nex::HttpEventListener* pListener, unsigned int chunkSize)
{
    ScopedCS lock(Core::GetInstance()->m_pScheduler->m_Lock);
    m_CallContextId = callContextId;
    m_URL = url;
    m_Method = method;
    m_pListener = pListener;
    m_ChunkSize = chunkSize;
    SetStepInline(&JobHttp::StepStart, "");
    return qResult(true);
}

// 0x003D1930 | fefates:bytes-fuzzy [tier B]
void nn::nex::JobHttp::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(result.m_Code >= 0 ? CallContext::STATE_SUCCESS : CallContext::STATE_FAILURE, result, true);
    }
    if (m_pConnection != nullptr) {
        delete m_pConnection;
        s_ConnectionCount--;
        if (s_pConnectionLock != nullptr) {
            s_pConnectionLock->Leave();
        }
        m_pConnection = nullptr;
    }
    SetState(STATE_COMPLETED);
}

// 0x003D19E0 (name is ours)
nn::nex::qResult nn::nex::JobHttp::StartConnection()
{
    if (s_pConnectionLock != nullptr) {
        s_pConnectionLock->Enter();
    }
    s_ConnectionCount++;
    m_pConnection = new CTRHttpConnection();
    char* pURL;
    m_URL.CreateCopy(&pURL);
    qResult result = m_pConnection->Initialize(pURL, static_cast<HttpConnection::Method>(m_Method));
    String::ReleaseBuffer(pURL);
    if (!(result.m_Code >= 0)) {
        return result;
    }
    m_pListener->PrepareRequest(m_pConnection);
    if (GetCallContext(m_CallContextId) == nullptr) {
        return qResult(RESULT_NO_CALL_CONTEXT);
    }
    result = m_pConnection->Start();
    if (result.m_Code >= 0) {
        return qResult(qResult::SUCCESS);
    }
    return result;
}

// 0x003D1B68 (name is ours)
void nn::nex::JobHttp::StepTransfer()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_NO_CALL_CONTEXT));
        return;
    }
    // the chunked post data
    int count = 0;
    while (m_pConnection->IsChunked() && !m_pConnection->IsPostDataFinished()) {
        if (count++ > MAX_POST_PARTS) {
            break;
        }
        u32 size = m_ChunkSize;
        if (m_pConnection->GetPostDataFreeSize() < size) {
            size = m_pConnection->GetPostDataFreeSize();
        }
        if (size == 0) {
            break;
        }
        qVector<u8> buffer(size);
        bool isLast = false;
        u32 written = m_pListener->PostChunkedBuffer(m_pConnection, buffer.begin(), buffer.size(), &isLast);
        if (written > size) {
            CompleteJob(qResult(RESULT_NO_CALL_CONTEXT));
            return;
        }
        if (GetCallContext(m_CallContextId) == nullptr) {
            CompleteJob(qResult(RESULT_NO_CALL_CONTEXT));
            return;
        }
        buffer.resize(written, 0);
        if (buffer.begin() != buffer.end()) {
            qResult result = m_pConnection->PushPostData(buffer.begin(), buffer.size());
            if (!(result.m_Code >= 0)) {
                CompleteJob(result);
                return;
            }
        }
        if (isLast) {
            m_pConnection->FinishPostData();
            break;
        }
        if (written == 0) {
            break;
        }
    }
    // the status
    if (!m_HasStatus) {
        qResult result = m_pConnection->GetStatus();
        if (!(result.m_Code >= 0)) {
            if (result.m_Code == RESULT_NOT_YET) {
                SetState(STATE_WAITING);
                m_WaitTime = m_PollTime;
                return;
            }
            if (result.m_Code == RESULT_CONNECTION_FAILED) {
                CompleteJob(qResult(RESULT_CONNECTION_FAILED));
            } else {
                CompleteJob(result);
            }
            return;
        }
        m_HasStatus = true;
        int statusCode;
        bool hasStatusCode = m_pConnection->GetStatusCode(&statusCode);
        if (!hasStatusCode || statusCode == 0) {
            CompleteJob(qResult(RESULT_NO_STATUS));
            return;
        }
        m_pListener->ProcessResponseHeader(m_pConnection, statusCode);
        if (GetCallContext(m_CallContextId) == nullptr) {
            CompleteJob(qResult(RESULT_NO_CALL_CONTEXT));
            return;
        }
        m_pConnection->SetUnknown0x10CE();
    }
    // the body
    qResult result = ReceiveBody();
    if (result.m_Code == RESULT_WAITING) {
        SetState(STATE_WAITING);
        m_WaitTime = m_PollTime;
        return;
    }
    CompleteJob(result);
}

// 0x003D1FC4 (name is ours)
nn::nex::qResult nn::nex::JobHttp::ReceiveBody()
{
    if (m_Buffer.begin() != m_Buffer.end()) {
        // the rest of the last part
        bool isTaken = m_pListener->ProcessResponse(m_Buffer.begin(), m_Buffer.size());
        if (GetCallContext(m_CallContextId) == nullptr) {
            return qResult(RESULT_NO_CALL_CONTEXT);
        }
        if (!isTaken) {
            return qResult(RESULT_WAITING);
        }
        m_Buffer.clear();
    }
    for (;;) {
        u32 size = 0;
        m_Buffer.resize(m_ChunkSize, 0);
        qResult result = m_pConnection->ReadBody(m_Buffer.begin(), &size, m_Buffer.size());
        if (!(result.m_Code >= 0)) {
            return result;
        }
        m_Buffer.resize(size, 0);
        if (m_Buffer.begin() == m_Buffer.end()) {
            if (m_pConnection->IsFinished()) {
                return qResult(qResult::SUCCESS);
            }
            return qResult(RESULT_WAITING);
        }
        bool isTaken = m_pListener->ProcessResponse(m_Buffer.begin(), m_Buffer.size());
        if (GetCallContext(m_CallContextId) == nullptr) {
            return qResult(RESULT_NO_CALL_CONTEXT);
        }
        if (!isTaken) {
            return qResult(RESULT_WAITING);
        }
        m_Buffer.clear();
    }
}

// 0x003D217C (name is ours)
void nn::nex::JobHttp::StepStart()
{
    if (GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_NO_CALL_CONTEXT));
        return;
    }
    qResult result = StartConnection();
    if (!(result.m_Code >= 0)) {
        CompleteJob(result);
        return;
    }
    SetStepInline(&JobHttp::StepTransfer, "");
}

} // namespace nex
} // namespace nn
