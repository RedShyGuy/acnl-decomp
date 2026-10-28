#include "nn/nex/nex_CTRHttpConnection.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_ThreadVariableList.h"
#include "nn/nex/nex_ThreadVariableRoot.h"
#include "nn/nex/nex_Time.h"
#include "nn/os/os_Api.h"

#include <cstdio>
#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_HTTP_FAILED = static_cast<int>(0x80050001);
const int RESULT_HTTP_TIMEOUT = static_cast<int>(0x80050009);
const int RESULT_HTTP_0x14 = static_cast<int>(0x80050014);
const int RESULT_HTTP_0x15 = static_cast<int>(0x80050015);
// the results of nn::http (after 3dbrew)
const bit32 HTTP_RESULT_0x46 = 0xD8A0A046;
const bit32 HTTP_RESULT_0x47 = 0xD8A0A047;
const bit32 HTTP_RESULT_0x48 = 0xD8A0A048;
const bit32 HTTP_RESULT_0x50 = 0xD8A0A050;
const bit32 HTTP_RESULT_SSL_ERROR = 0xD8A0A03C;
// the body did not fit into the buffer (more follows)
const bit32 HTTP_RESULT_BODY_BUFFER_SHORTAGE = 0xD840A02B;
// the result before anything was read
const bit32 HTTP_RESULT_NONE = 0xE7E3FFFF;
// the internal root CAs and the client certificate
const u32 DEFAULT_ROOT_CA_FIRST = 1;
const u32 DEFAULT_ROOT_CA_LAST = 5;
const u32 ROOT_CA_FIRST = 6;
const u32 ROOT_CA_LAST = 10;
const u32 CLIENT_CERT_ID = 0x40;
// the poll interval of the thread (ns) and the time it waits for the finalization (ms)
const s64 POLL_INTERVAL = 1000000;
const s64 FINALIZE_WAIT_TIME = 100;
} // namespace

// 0x003858AC
nn::nex::CTRHttpConnection::CTRHttpConnection() : m_Result(qResult::SUCCESS), m_Lock(0x20000000)
{
    m_IsDone = false;
    m_IsCanceled = false;
    m_IsPostDataFinished = false;
    m_IsPostDataSent = false;
    m_HasStatusCode = false;
    m_StatusCode = 0;
    memset(m_ContentLength, 0, sizeof(m_ContentLength));
    m_BufferSize = 0x4000;
    m_pQueue = nullptr;
    m_PostDataSize = 0;
    m_ThreadId = 0;
    m_IsThreadStarted = false;
    m_IsFinalized = false;
    m_Unknown0x10CE = 0;
}

// 0x003859E4 slot 0x00 | fefates:callseq
// 0x003859B4 (deleting dtor)
nn::nex::CTRHttpConnection::~CTRHttpConnection()
{
    Cancel();
    if (m_Thread.GetHandle().IsValid()) {
        m_Thread.Join();
    }
    if (m_pQueue != nullptr) {
        delete m_pQueue;
    }
    if (m_IsThreadStarted) {
        ThreadVariableList::s_pInstance->ClearThread(m_ThreadId);
    }
    if (!m_IsFinalized) {
        m_Connection.Finalize();
    }
}

// 0x0038483C slot 0x08
nn::nex::qResult nn::nex::CTRHttpConnection::Initialize(const char* pURL, Method method)
{
    ScopedCS lock(m_Lock);
    nn::http::RequestMethod requestMethod;
    switch (method) {
    case METHOD_HEAD:
        requestMethod = nn::http::REQUEST_METHOD_HEAD;
        break;
    case METHOD_POST:
        requestMethod = nn::http::REQUEST_METHOD_POST;
        break;
    case METHOD_GET:
    default:
        requestMethod = nn::http::REQUEST_METHOD_GET;
        break;
    }
    if (m_Connection.Initialize(pURL, requestMethod, true).IsFailure()) {
        return qResult(RESULT_HTTP_FAILED);
    }
    return qResult(qResult::SUCCESS);
}

// 0x00384DFC slot 0x0C
nn::nex::qResult nn::nex::CTRHttpConnection::AddHeaderField(const char* pLabel, const char* pValue)
{
    ScopedCS lock(m_Lock);
    return ConvertResult(m_Connection.AddHeaderField(pLabel, pValue));
}

// 0x00384964 slot 0x10
nn::nex::qResult nn::nex::CTRHttpConnection::AddPostDataAscii(const char* pLabel, const char* pValue)
{
    ScopedCS lock(m_Lock);
    return ConvertResult(m_Connection.AddPostDataAscii(pLabel, pValue));
}

// 0x00384E58 slot 0x14
void nn::nex::CTRHttpConnection::SetChunkedPostData(unsigned int size)
{
    ScopedCS lock(m_Lock);
    m_IsChunked = true;
    m_PostDataSize = size;
}

// 0x0038502C slot 0x1C
void nn::nex::CTRHttpConnection::FinishPostData()
{
    ScopedCS lock(m_Lock);
    m_IsPostDataFinished = true;
}

// 0x0072C0E8 slot 0x20
bool nn::nex::CTRHttpConnection::IsPostDataFinished()
{
    ScopedCS lock(m_Lock);
    return m_IsPostDataFinished;
}

// 0x003849C0 slot 0x24 | mk7dlp:callseq
nn::nex::qResult nn::nex::CTRHttpConnection::Start()
{
    ScopedCS lock(m_Lock);
    if (m_UsesRootCas) {
        for (u32 id = DEFAULT_ROOT_CA_FIRST; id <= DEFAULT_ROOT_CA_LAST; id++) {
            if (m_Connection.SetInternalRootCa(id).IsFailure()) {
                return qResult(RESULT_HTTP_FAILED);
            }
        }
    }
    for (u32 id = ROOT_CA_FIRST; id <= ROOT_CA_LAST; id++) {
        if (m_Connection.SetInternalRootCa(id).IsFailure()) {
            return qResult(RESULT_HTTP_FAILED);
        }
    }
    if (m_UsesClientCert && m_Connection.SetInternalClientCert(CLIENT_CERT_ID).IsFailure()) {
        return qResult(RESULT_HTTP_FAILED);
    }
    if (m_IsChunked) {
        // a multipart post
        m_Connection.SetLazyPostDataSetting(nn::http::POST_DATA_TYPE_RAW, m_PostDataSize);
        char contentType[128];
        snprintf(contentType, sizeof(contentType), "multipart/form-data; boundary=%s", GetBoundaryString());
        m_Connection.AddHeaderField("Content-Type", contentType);
    }
    nn::Result result = m_Connection.StartConnectionAsync();
    if (result.IsFailure()) {
        return ConvertResult(result);
    }
    // the buffers and the thread
    m_Data.reserve(m_BufferSize);
    m_pQueue = new qReservedQueue<u8>(m_BufferSize);
    result = m_Thread.TryStart(&ThreadProc, reinterpret_cast<uptr>(this), reinterpret_cast<uptr>(m_Stack + STACK_SIZE), m_ThreadPriority, nn::os::Thread::CORE_NO_DEFAULT);
    if (result.IsFailure()) {
        m_Connection.Cancel();
        return ConvertResult(result);
    }
    m_IsThreadStarted = true;
    return qResult(qResult::SUCCESS);
}

// 0x00385804 slot 0x28
nn::nex::qResult nn::nex::CTRHttpConnection::Cancel()
{
    ScopedCS lock(m_Lock);
    m_IsCanceled = true;
    if (m_IsFinalized) {
        return qResult(qResult::SUCCESS);
    }
    return ConvertResult(m_Connection.Cancel());
}

// 0x0072BD78 slot 0x2C
bool nn::nex::CTRHttpConnection::GetStatusCode(int* pStatusCode)
{
    ScopedCS lock(m_Lock);
    if (!GetStatus()) {
        return false;
    }
    *pStatusCode = m_StatusCode;
    return true;
}

// 0x003856B4 slot 0x30
nn::nex::qResult nn::nex::CTRHttpConnection::ReadBody(unsigned char* pBuffer, unsigned int* pSize, unsigned int maxSize)
{
    ScopedCS lock(m_Lock);
    if (m_IsChunked && !m_IsPostDataSent) {
        *pSize = 0;
        return qResult(qResult::SUCCESS);
    }
    if (!m_Result) {
        return m_Result;
    }
    u32 count = m_pQueue->m_Count;
    u32 size = count > maxSize ? maxSize : count;
    *pSize = size;
    if (size != 0) {
        m_pQueue->PopInline(pBuffer, size);
    }
    return qResult(qResult::SUCCESS);
}

// 0x00384EA4 slot 0x34
nn::nex::qResult nn::nex::CTRHttpConnection::PushPostData(const unsigned char* pData, unsigned int size)
{
    ScopedCS lock(m_Lock);
    if (!m_Result) {
        return m_Result;
    }
    if (m_IsPostDataFinished || size > m_pQueue->GetFreeSize()) {
        return qResult(RESULT_HTTP_FAILED);
    }
    if (size != 0) {
        m_pQueue->push(pData, size);
    }
    return qResult(qResult::SUCCESS);
}

// 0x0072BE0C slot 0x38
unsigned int nn::nex::CTRHttpConnection::GetPostDataFreeSize()
{
    ScopedCS lock(m_Lock);
    if (m_IsPostDataFinished) {
        return 0;
    }
    return m_pQueue->GetFreeSize();
}

// 0x0072C1BC slot 0x3C
nn::nex::qResult nn::nex::CTRHttpConnection::GetResult() const
{
    return m_Result;
}

// 0x0072C12C slot 0x40
nn::nex::qResult nn::nex::CTRHttpConnection::GetStatus()
{
    ScopedCS lock(m_Lock);
    if (!m_Result) {
        return m_Result;
    }
    return qResult(m_HasStatusCode ? qResult::SUCCESS : qResult::FAILURE);
}

// 0x0072BD04 slot 0x44
bool nn::nex::CTRHttpConnection::IsFinished()
{
    ScopedCS lock(m_Lock);
    return m_pQueue->m_Count == 0 && m_Data.begin() == m_Data.end() && m_IsDone;
}

// 0x00384900 slot 0x48
nn::nex::qResult nn::nex::CTRHttpConnection::AddPostDataBinary(const char* pLabel, const unsigned char* pData, unsigned int size)
{
    ScopedCS lock(m_Lock);
    return ConvertResult(m_Connection.AddPostDataBinary(pLabel, pData, size));
}

// 0x0072BE78 slot 0x4C
bool nn::nex::CTRHttpConnection::GetHeaderField(const char* pLabel, nn::nex::String* pValue)
{
    ScopedCS lock(m_Lock);
    if (!GetStatus() || m_Unknown0x10CE != 0) {
        return false;
    }
    if (strcmp(pLabel, "Content-Length") == 0) {
        // (from the response that the thread received)
        const char* pContentLength = m_ContentLength;
        if (strcmp(pContentLength, "") == 0) {
            return false;
        }
        String::ReleaseBuffer(pValue->m_pString);
        String::CopyUtf8String(&pValue->m_pString, pContentLength);
        return true;
    }
    size_t size = 0;
    if (m_Connection.GetHeaderField(pLabel, nullptr, 0, &size).IsFailure()) {
        return false;
    }
    u32 bufferSize = size + 1;
    if (bufferSize > 0x1000) {
        return false;
    }
    char* pBuffer = static_cast<char*>(MemoryManager::Allocate(bufferSize));
    memset(pBuffer, 0, bufferSize);
    bool isFound = m_Connection.GetHeaderField(pLabel, pBuffer, bufferSize, nullptr).IsSuccess();
    if (isFound) {
        String::ReleaseBuffer(pValue->m_pString);
        String::CopyUtf8String(&pValue->m_pString, pBuffer);
    }
    if (pBuffer != nullptr) {
        MemoryManager::FreeInline(pBuffer);
    }
    return isFound;
}

// 0x00385070 slot 0x50
void nn::nex::CTRHttpConnection::SetUnknown0x10CE()
{
    ScopedCS lock(m_Lock);
    m_Unknown0x10CE = 1;
}

// 0x00385894 slot 0x54 | mk7dlp:bytes
bool nn::nex::CTRHttpConnection::AddRootCa(const unsigned char* pCertificate, int size)
{
    return m_Connection.SetRootCa(pCertificate, size).IsSuccess();
}

// 0x003850B4 slot 0x58
void nn::nex::CTRHttpConnection::SetBufferSize(unsigned int size)
{
    m_BufferSize = size;
}

// 0x00384F94
nn::nex::qResult nn::nex::CTRHttpConnection::ConvertResult(nn::Result result)
{
    if (result.IsSuccess()) {
        return qResult(qResult::SUCCESS);
    }
    bit32 value = result.GetPrintableBits();
    if (value == HTTP_RESULT_0x46) {
        return qResult(RESULT_HTTP_0x14);
    }
    if (value == HTTP_RESULT_0x50) {
        return qResult(RESULT_HTTP_0x15);
    }
    if (value == HTTP_RESULT_0x47 || value == HTTP_RESULT_0x48) {
        return qResult(RESULT_HTTP_TIMEOUT);
    }
    return qResult(RESULT_HTTP_FAILED);
}

// 0x003850C0
void nn::nex::CTRHttpConnection::ThreadProc(uptr arg)
{
    reinterpret_cast<CTRHttpConnection*>(arg)->Run();
}

// 0x003850C4
void nn::nex::CTRHttpConnection::Run()
{
    // (the lock is called through its vtable)
    CriticalSection* pLock = &m_Lock;
    u32 lastReceived = 0;
    m_ThreadId = ThreadVariableRoot::GetCurrentThreadId();
    bool hasStatus = false;
    bool isReceived = false;
    for (;;) {
        if (isReceived && m_Data.begin() == m_Data.end()) {
            break;
        }
        if (m_IsCanceled) {
            break;
        }
        OsSleepThread(POLL_INTERVAL);
        if (m_IsChunked && !m_IsPostDataSent) {
            // the post data
            pLock->Enter();
            if (m_pQueue->m_Count != 0) {
                m_Data.resize(m_pQueue->m_Count, 0);
                m_pQueue->PopInline(m_Data.begin(), m_Data.size());
                m_pQueue->Clear();
            }
            m_IsPostDataSent = m_IsPostDataFinished;
            pLock->Leave();
            if (m_Data.begin() != m_Data.end()) {
                nn::Result result = m_Connection.SendPostDataRaw(m_Data.begin(), m_Data.size());
                if (result.IsFailure()) {
                    m_Result = ConvertResult(result);
                    break;
                }
                m_Data.clear();
            }
            if (!m_IsPostDataSent) {
                continue;
            }
            m_Connection.NotifyFinishSendPostData();
        }
        // the body
        nn::Result result(HTTP_RESULT_NONE);
        bool isRead = false;
        if (!isReceived && m_Data.begin() == m_Data.end()) {
            m_Data.resize(m_BufferSize, 0);
            result = m_Connection.ReadBody(m_Data.begin(), m_Data.size());
            isRead = true;
        }
        if (isRead && !hasStatus && (result.IsSuccess() || result.GetPrintableBits() == HTTP_RESULT_BODY_BUFFER_SHORTAGE)) {
            int statusCode = 0;
            nn::Result statusResult = m_Connection.GetStatusCode(&statusCode);
            if (statusResult.IsFailure()) {
                m_Result = ConvertResult(statusResult);
                break;
            }
            char contentLength[32];
            if (m_Connection.GetHeaderField("Content-Length", contentLength, sizeof(contentLength), nullptr).IsSuccess()) {
                pLock->Enter();
                strncpy(m_ContentLength, contentLength, sizeof(m_ContentLength));
                m_ContentLength[sizeof(m_ContentLength) - 1] = '\0';
                pLock->Leave();
            }
            hasStatus = true;
            pLock->Enter();
            m_HasStatusCode = true;
            m_StatusCode = statusCode;
            pLock->Leave();
        }
        pLock->Enter();
        if (isRead) {
            if (result.IsFailure() && result.GetPrintableBits() != HTTP_RESULT_BODY_BUFFER_SHORTAGE) {
                if (!m_IsCanceled) {
                    if (result.GetPrintableBits() == HTTP_RESULT_SSL_ERROR) {
                        int sslError = 0;
                        m_Connection.GetSslError(&sslError);
                    }
                    m_Result = ConvertResult(result);
                }
                pLock->Leave();
                break;
            }
            u32 received = 0;
            u32 total = 0;
            m_Connection.GetProgress(&received, &total);
            u32 size = received - lastReceived;
            lastReceived = received;
            m_Data.resize(size, 0);
            if (result.IsSuccess()) {
                isReceived = true;
            }
        }
        // to the queue (when there is room)
        if (m_Data.begin() != m_Data.end() && m_Data.size() <= m_pQueue->GetFreeSize()) {
            m_pQueue->PushInline(m_Data.begin(), m_Data.size());
            m_Data.clear();
        }
        pLock->Leave();
    }
    // the connection is finalized when SetUnknown0x10CE was called (it waits a little for it)
    Time start = SystemClock::GetTime();
    while (static_cast<s64>(SystemClock::GetTime().m_Value - start.m_Value) < FINALIZE_WAIT_TIME) {
        pLock->Enter();
        s8 unknown0x10CE = m_Unknown0x10CE;
        pLock->Leave();
        if (unknown0x10CE != 0) {
            m_Connection.Finalize();
            m_IsFinalized = true;
            break;
        }
        OsSleepThread(POLL_INTERVAL);
    }
    m_IsDone = true;
}

} // namespace nex
} // namespace nn
