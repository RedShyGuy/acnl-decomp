#include "nn/nex/nex_JobDataStoreGetObject.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreFileServerClient.h"
#include "nn/nex/nex_DataStoreLogicServerClientInterface.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Time.h"

#include <cstdlib>
#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_BUFFER_TOO_SMALL = static_cast<int>(0x80050017);
const int RESULT_INVALID_SIZE = static_cast<int>(0x80690001);
const int RESULT_NO_META_BINARY = static_cast<int>(0x8069000B);
// the timeout without a minimum speed
const u32 NO_TIMEOUT = 0x7FFFFFFF;
// the wait before the listener gets the meta binary again (ms)
const int LISTENER_WAIT_TIME = 16;

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

// (inline) PrepareGetObject of the version of the parameters
inline bool PrepareGetObject(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStorePrepareGetParamV1& param,
                             DataStoreReqGetInfoV1* pReqGetInfo)
{
    return pClient->PrepareGetObjectV1(pContext, param, pReqGetInfo);
}
inline bool PrepareGetObject(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStorePrepareGetParam& param,
                             DataStoreReqGetInfo* pReqGetInfo)
{
    return pClient->PrepareGetObject(pContext, param, pReqGetInfo);
}

// (inline) PrepareGetObjectOrMetaBinary of the version of the parameters
inline bool PrepareGetObjectOrMetaBinary(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStorePrepareGetParamV1& param,
                                         DataStoreReqGetInfoV1* pReqGetInfo, DataStoreReqGetAdditionalMeta* pAdditionalMeta)
{
    return pClient->PrepareGetObjectOrMetaBinaryV1(pContext, param, pReqGetInfo, pAdditionalMeta);
}
inline bool PrepareGetObjectOrMetaBinary(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStorePrepareGetParam& param,
                                         DataStoreReqGetInfo* pReqGetInfo, DataStoreReqGetAdditionalMeta* pAdditionalMeta)
{
    return pClient->PrepareGetObjectOrMetaBinary(pContext, param, pReqGetInfo, pAdditionalMeta);
}
} // namespace

template <typename Traits>
nn::nex::qResult nn::nex::JobDataStoreGetObject<Traits>::Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient,
                                                                   const TPrepareGetParam& param, unsigned int threadPriority, bool usesHttps,
                                                                   unsigned int httpBufferSize, unsigned int minBytesPerSecond, unsigned int minTimeout,
                                                                   Listener* pListener, unsigned int chunkSize, unsigned char* pBuffer, unsigned int bufferSize,
                                                                   bool getsMeta, unsigned int* pSize, const TReqGetInfo* pReqGetInfo)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_pClient = pClient;
    m_pFileServerClient = new DataStoreFileServerClient();
    m_pListener = pListener;
    m_Param = param;
    m_ThreadPriority = threadPriority;
    m_UsesHttps = usesHttps;
    m_HttpBufferSize = httpBufferSize;
    m_ChunkSize = chunkSize;
    m_MinBytesPerSecond = minBytesPerSecond;
    m_MinTimeout = minTimeout;
    m_pBuffer = pBuffer;
    m_BufferSize = bufferSize;
    m_GetsMeta = getsMeta;
    m_pSize = pSize;
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    if (pReqGetInfo != nullptr) {
        // the request of the caller: no PrepareGetObject
        m_ReqGetInfo = *pReqGetInfo;
        StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreGetObject::StepFileServerGetObject, ""));
    } else {
        StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreGetObject::StepLogicServerPrepareGetObject, ""));
    }
    bool isSuccess = true;
    return qResult(isSuccess);
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            if (m_pSize != nullptr) {
                *m_pSize = m_ReqGetInfo.m_Size;
            }
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    Job::SetToComplete();
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::PrepareRequest(nn::nex::HttpConnection* pConnection)
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    pConnection->m_ThreadPriority = m_ThreadPriority;
    pConnection->SetBufferSize(m_HttpBufferSize);
    for (DataStoreKeyValue* pField = m_ReqGetInfo.m_RequestHeaders.begin(); pField != m_ReqGetInfo.m_RequestHeaders.end(); pField++) {
        char* pKey = nullptr;
        char* pValue = nullptr;
        pField->m_Key.CreateCopy(&pKey);
        pField->m_Value.CreateCopy(&pValue);
        pConnection->AddHeaderField(pKey, pValue);
        String::ReleaseCopy(pKey);
        String::ReleaseCopy(pValue);
    }
    if (m_ReqGetInfo.m_RootCaCert.size() != 0) {
        pConnection->AddRootCa(m_ReqGetInfo.m_RootCaCert.begin(), m_ReqGetInfo.m_RootCaCert.size());
    }
}

template <typename Traits>
bool nn::nex::JobDataStoreGetObject<Traits>::ProcessResponse(const unsigned char* pData, unsigned int size)
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        return false;
    }
    if (m_pListener != nullptr) {
        if (m_IsFirst) {
            m_IsFirst = false;
            TDataId dataId = m_Param.m_DataId;
            if (m_GetsMeta) {
                m_pListener->BeginInternal(dataId, m_ReqGetInfo.m_Size, m_AdditionalMeta.m_OwnerId, m_AdditionalMeta.m_DataType, m_AdditionalMeta.m_Version, true);
                if (GetCallerContext(m_CallContextId) == nullptr) {
                    return false;
                }
            }
            m_pListener->Begin(dataId, m_ReqGetInfo.m_Size);
            if (GetCallerContext(m_CallContextId) == nullptr) {
                return false;
            }
        }
        bool isProcessed = m_pListener->ProcessData(pData, size);
        if (isProcessed) {
            m_ReceivedSize += size;
        }
        return isProcessed;
    }
    if (m_pBuffer == nullptr) {
        return true;
    }
    if (m_ReceivedSize + size > m_BufferSize) {
        m_IsBufferTooSmall = true;
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
        return false;
    }
    memcpy(m_pBuffer + m_ReceivedSize, pData, size);
    m_ReceivedSize += size;
    return true;
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::ProcessResponseHeader(nn::nex::HttpConnection* pConnection, int statusCode)
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        return;
    }
    String contentLength;
    if (pConnection->GetHeaderField("Content-Length", &contentLength)) {
        char* pEnd = nullptr;
        char* pCopy = nullptr;
        contentLength.CreateCopy(&pCopy);
        u32 size = strtoul(pCopy, &pEnd, 10);
        String::ReleaseCopy(pCopy);
        if (size != 0 && m_ReqGetInfo.m_Size != size) {
            // (a message was logged: "JobDataStoreGetObject::ProcessResponseHeader: Invalid ...")
            m_IsSizeMismatch = true;
            m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
        }
    }
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::StepProcessMetaBinary()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    qBuffer& metaBinary = m_AdditionalMeta.m_MetaBinary;
    if (metaBinary.m_pEnd == metaBinary.m_pBegin) {
        CompleteJob(qResult(RESULT_NO_META_BINARY));
        return;
    }
    if (m_pListener != nullptr) {
        if (m_IsFirst) {
            m_IsFirst = false;
            TDataId dataId = m_Param.m_DataId;
            m_pListener->BeginInternal(dataId, metaBinary.size(), m_AdditionalMeta.m_OwnerId, m_AdditionalMeta.m_DataType, m_AdditionalMeta.m_Version, false);
            if (GetCallerContext(m_CallContextId) == nullptr) {
                CompleteJob(qResult(RESULT_CANCELLED));
                return;
            }
            m_pListener->Begin(dataId, metaBinary.size());
            if (GetCallerContext(m_CallContextId) == nullptr) {
                CompleteJob(qResult(RESULT_CANCELLED));
                return;
            }
        }
        if (!m_pListener->ProcessData(metaBinary.m_pBegin, metaBinary.size())) {
            // the listener gets the meta binary again
            Job::SetToWaiting(LISTENER_WAIT_TIME);
            return;
        }
        m_ReceivedSize += metaBinary.size();
        CompleteJob(qResult(RESULT_SUCCESS));
        return;
    }
    if (m_pBuffer == nullptr) {
        return;
    }
    if (metaBinary.m_pEnd - metaBinary.m_pBegin > m_BufferSize) {
        CompleteJob(qResult(RESULT_BUFFER_TOO_SMALL));
        return;
    }
    memcpy(m_pBuffer, metaBinary.m_pBegin, metaBinary.m_pEnd - metaBinary.m_pBegin);
    m_ReceivedSize += metaBinary.size();
    CompleteJob(qResult(RESULT_SUCCESS));
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::StepFileServerGetObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    bool hasScheme = m_ReqGetInfo.m_Url.FindSubstringNoCase(L"http://") == 0 || m_ReqGetInfo.m_Url.FindSubstringNoCase(L"https://") == 0;
    String url = hasScheme ? String(m_ReqGetInfo.m_Url) : (m_UsesHttps ? L"https://" : L"http://") + m_ReqGetInfo.m_Url;
    u32 timeout = NO_TIMEOUT;
    if (m_MinBytesPerSecond != 0) {
        u32 getTime = m_ReqGetInfo.m_Size / m_MinBytesPerSecond * 1000;
        timeout = static_cast<s32>(getTime) < static_cast<s32>(m_MinTimeout) ? m_MinTimeout : getTime;
    }
    m_ProtocolCallContext.m_Deadline = Time::ConvertTimeoutToDeadline(timeout);
    qResult result = m_pFileServerClient->ApiImpl(&m_ProtocolCallContext, url, this, HttpConnection::METHOD_GET, m_ChunkSize);
    if (!result) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext, new StepSequenceJob::Step(&JobDataStoreGetObject::StepWaitingFileServerGetObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::StepWaitingFileServerGetObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        if (m_IsBufferTooSmall) {
            CompleteJob(qResult(RESULT_BUFFER_TOO_SMALL));
        } else if (m_IsSizeMismatch) {
            CompleteJob(qResult(RESULT_INVALID_SIZE));
        } else {
            CompleteJob(m_ProtocolCallContext.m_Result);
        }
        return;
    }
    if (m_ReceivedSize != m_ReqGetInfo.m_Size) {
        CompleteJob(qResult(RESULT_INVALID_SIZE));
        return;
    }
    CompleteJob(qResult(RESULT_SUCCESS));
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::StepLogicServerPrepareGetObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    bool isCalled;
    if (m_GetsMeta) {
        isCalled = PrepareGetObjectOrMetaBinary(m_pClient, &m_ProtocolCallContext, m_Param, &m_ReqGetInfo, &m_AdditionalMeta);
    } else {
        isCalled = PrepareGetObject(m_pClient, &m_ProtocolCallContext, m_Param, &m_ReqGetInfo);
    }
    if (!isCalled) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext, new StepSequenceJob::Step(&JobDataStoreGetObject::StepWaitingLogicServerPrepareGetObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreGetObject<Traits>::StepWaitingLogicServerPrepareGetObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        // (a message was logged: "Failed:PrepareGetObject 0x%x")
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    m_ProtocolCallContext.Reset();
    if (m_GetsMeta && String::IsEqual(m_ReqGetInfo.m_Url.m_pString, L"")) {
        // no data: only the meta binary
        StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreGetObject::StepProcessMetaBinary, ""));
    } else {
        StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreGetObject::StepFileServerGetObject, ""));
    }
}

template <typename Traits>
nn::nex::JobDataStoreGetObject<Traits>::JobDataStoreGetObject()
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pClient(nullptr), m_pFileServerClient(nullptr), m_UsesHttps(true),
      m_HttpBufferSize(0), m_ThreadPriority(0), m_MinBytesPerSecond(0), m_MinTimeout(0), m_pListener(nullptr), m_ChunkSize(0), m_pBuffer(nullptr), m_BufferSize(0),
      m_GetsMeta(false), m_pSize(nullptr), m_ReceivedSize(0), m_IsBufferTooSmall(false), m_IsSizeMismatch(false), m_IsFirst(true)
{
    // nothing to do
}

template <typename Traits>
nn::nex::JobDataStoreGetObject<Traits>::~JobDataStoreGetObject()
{
    if (m_pFileServerClient != nullptr) {
        delete m_pFileServerClient;
    }
}

typedef JobDataStoreGetObject<DataStoreApiVersionTraitsV0> JobDataStoreGetObjectV0;
typedef JobDataStoreGetObject<DataStoreApiVersionTraitsV1> JobDataStoreGetObjectV1;
ASSERT_OFFSET(JobDataStoreGetObjectV0, m_ProtocolCallContext, 0x98);
ASSERT_OFFSET(JobDataStoreGetObjectV0, m_ReqGetInfo, 0x120);
ASSERT_OFFSET(JobDataStoreGetObjectV0, m_AdditionalMeta, 0x14C);
ASSERT_OFFSET(JobDataStoreGetObjectV0, m_pBuffer, 0x17C);
ASSERT_OFFSET(JobDataStoreGetObjectV0, m_IsFirst, 0x192);
ASSERT_OFFSET(JobDataStoreGetObjectV1, m_ReqGetInfo, 0x150);
ASSERT_OFFSET(JobDataStoreGetObjectV1, m_AdditionalMeta, 0x188);
ASSERT_OFFSET(JobDataStoreGetObjectV1, m_pBuffer, 0x1B8);
ASSERT_OFFSET(JobDataStoreGetObjectV1, m_IsFirst, 0x1CE);

// 0x007DC084 | fefates:bytes [tier B]
template nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::JobDataStoreGetObject();
// 0x007DC19C slot 0x00 | fefates:bytes
// 0x007DC18C (deleting dtor)
// 0x0077204C (thunk)
// 0x00772038 (deleting thunk)
template nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::~JobDataStoreGetObject();
// 0x007DB234 slot 0x34
template nn::nex::qResult nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::Initialize(
    unsigned int, nn::nex::DataStoreLogicServerClientInterface*, const nn::nex::DataStorePrepareGetParamV1&, unsigned int, bool, unsigned int, unsigned int, unsigned int,
    nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*, unsigned int, unsigned char*, unsigned int, bool, unsigned int*, const nn::nex::DataStoreReqGetInfoV1*);
// 0x007DB3FC | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::CompleteJob(const nn::nex::qResult&);
// 0x007DB498 slot 0x38
// 0x00771E44 (thunk)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::PrepareRequest(nn::nex::HttpConnection*);
// 0x007DB5A4 slot 0x40 | fefates:bytes [tier B]
// 0x00771F50 (thunk)
template bool nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::ProcessResponse(const unsigned char*, unsigned int);
// 0x007DB71C slot 0x3C
// 0x00771F60 (thunk)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::ProcessResponseHeader(nn::nex::HttpConnection*, int);
// 0x007DB7F4 | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepProcessMetaBinary();
// 0x007DBA54 (name is ours)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepFileServerGetObject();
// 0x007DBC64 | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingFileServerGetObject();
// 0x007DBDB4 | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepLogicServerPrepareGetObject();
// 0x007DBF14 (name is ours)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingLogicServerPrepareGetObject();

// 0x007DD10C | fefates:callseq
template nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::JobDataStoreGetObject();
// 0x007DD21C slot 0x00 | fefates:bytes
// 0x007DD20C (deleting dtor)
// 0x00772264 (thunk)
// 0x00772250 (deleting thunk)
template nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::~JobDataStoreGetObject();
// 0x007DC22C slot 0x34
template nn::nex::qResult nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::Initialize(
    unsigned int, nn::nex::DataStoreLogicServerClientInterface*, const nn::nex::DataStorePrepareGetParam&, unsigned int, bool, unsigned int, unsigned int, unsigned int,
    nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*, unsigned int, unsigned char*, unsigned int, bool, unsigned int*, const nn::nex::DataStoreReqGetInfo*);
// 0x007DC438 | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::CompleteJob(const nn::nex::qResult&);
// 0x007DC4D4 slot 0x38
// 0x0077205C (thunk)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::PrepareRequest(nn::nex::HttpConnection*);
// 0x007DC5E0 slot 0x40 | fefates:bytes [tier B]
// 0x00772168 (thunk)
template bool nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::ProcessResponse(const unsigned char*, unsigned int);
// 0x007DC780 slot 0x3C
// 0x00772178 (thunk)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::ProcessResponseHeader(nn::nex::HttpConnection*, int);
// 0x007DC858 | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepProcessMetaBinary();
// 0x007DCADC (name is ours)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepFileServerGetObject();
// 0x007DCCEC | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingFileServerGetObject();
// 0x007DCE3C | fefates:bytes [tier B]
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepLogicServerPrepareGetObject();
// 0x007DCF9C (name is ours)
template void nn::nex::JobDataStoreGetObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingLogicServerPrepareGetObject();

} // namespace nex
} // namespace nn
