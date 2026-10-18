#include "nn/nex/nex_JobDataStorePostObject.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreFileServerClient.h"
#include "nn/nex/nex_DataStoreLogicServerClientInterface.h"
#include "nn/nex/nex_DataStorePostHttpEventListener.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_Time.h"

#include <cstring>

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
// the timeout without a minimum speed
const u32 NO_TIMEOUT = 0x7FFFFFFF;
// the flag of the parameters: no CompletePostObject (the name is ours)
const u32 FLAG_NO_COMPLETE_POST = 0x40;

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

// (inline) PreparePostObject of the version of the parameters
inline bool PreparePostObject(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStorePreparePostParamV1& param,
                              DataStoreReqPostInfoV1* pReqPostInfo)
{
    return pClient->PreparePostObjectV1(pContext, param, pReqPostInfo);
}
inline bool PreparePostObject(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStorePreparePostParam& param,
                              DataStoreReqPostInfo* pReqPostInfo)
{
    return pClient->PreparePostObject(pContext, param, pReqPostInfo);
}

// (inline) CompletePostObject of the version of the parameters
inline bool CompletePostObject(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStoreCompletePostParamV1& param)
{
    return pClient->CompletePostObjectV1(pContext, param);
}
inline bool CompletePostObject(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStoreCompletePostParam& param)
{
    return pClient->CompletePostObject(pContext, param);
}
} // namespace

template <typename Traits>
nn::nex::qResult nn::nex::JobDataStorePostObject<Traits>::Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient,
                                                                    const TPreparePostParam& param, unsigned int threadPriority, bool usesHttps, unsigned int bufferSize,
                                                                    unsigned int minBytesPerSecond, unsigned int minTimeout, TDataId* pDataId,
                                                                    nn::nex::DataStorePostObjectEventListener* pListener, unsigned int chunkSize,
                                                                    const unsigned char* pData, bool copiesData)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_pClient = pClient;
    m_pFileServerClient = new DataStoreFileServerClient();
    m_pListener = pListener;
    m_Param = param;
    m_ThreadPriority = threadPriority;
    m_UsesHttps = usesHttps;
    m_BufferSize = bufferSize;
    m_ChunkSize = chunkSize;
    m_pDataId = pDataId;
    m_MinBytesPerSecond = minBytesPerSecond;
    m_MinTimeout = minTimeout;
    if (pData != nullptr) {
        if (copiesData) {
            m_pDataCopy = static_cast<u8*>(MemoryManager::Allocate(param.m_Size));
            memcpy(m_pDataCopy, pData, param.m_Size);
        } else {
            m_pData = pData;
        }
    }
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStorePostObject::StepLogicServerPreparePostObject, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

template <typename Traits>
void nn::nex::JobDataStorePostObject<Traits>::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            *m_pDataId = m_ReqPostInfo.m_DataId;
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
void nn::nex::JobDataStorePostObject<Traits>::StepLogicServerPreparePostObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    if (!PreparePostObject(m_pClient, &m_ProtocolCallContext, m_Param, &m_ReqPostInfo)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext, new StepSequenceJob::Step(&JobDataStorePostObject::StepFileServerPostObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStorePostObject<Traits>::StepFileServerPostObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        // (a message was logged: "Failed:PreparePostObject 0x%x")
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    m_ProtocolCallContext.Reset();
    m_pHttpListener = new DataStorePostHttpEventListener(m_CallContextId, m_ReqPostInfo.m_RequestHeaders, m_ReqPostInfo.m_FormFields, m_ReqPostInfo.m_RootCaCert,
                                                         m_ThreadPriority, m_BufferSize, m_pDataCopy != nullptr ? m_pDataCopy : m_pData, m_Param.m_Size, m_pListener);
    bool hasScheme = m_ReqPostInfo.m_Url.FindSubstringNoCase(L"http://") == 0 || m_ReqPostInfo.m_Url.FindSubstringNoCase(L"https://") == 0;
    String url = hasScheme ? String(m_ReqPostInfo.m_Url) : (m_UsesHttps ? L"https://" : L"http://") + m_ReqPostInfo.m_Url;
    u32 timeout = NO_TIMEOUT;
    if (m_MinBytesPerSecond != 0) {
        u32 postTime = m_Param.m_Size / m_MinBytesPerSecond * 1000;
        timeout = static_cast<s32>(postTime) < static_cast<s32>(m_MinTimeout) ? m_MinTimeout : postTime;
    }
    m_ProtocolCallContext.m_Deadline = Time::ConvertTimeoutToDeadline(timeout);
    qResult result = m_pFileServerClient->ApiImpl(&m_ProtocolCallContext, url, m_pHttpListener, HttpConnection::METHOD_POST, m_ChunkSize);
    if (!result) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext, new StepSequenceJob::Step(&JobDataStorePostObject::StepLogicServerCompletePostObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStorePostObject<Traits>::StepLogicServerCompletePostObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        m_PostResult = m_ProtocolCallContext.m_Result;
    }
    if (m_Param.m_Flag & FLAG_NO_COMPLETE_POST) {
        CompleteJob(m_PostResult);
        return;
    }
    m_ProtocolCallContext.Reset();
    // the logic server gets the result of the post
    TCompletePostParam param;
    param.m_DataId = m_ReqPostInfo.m_DataId;
    param.m_IsSuccess = m_PostResult;
    if (!CompletePostObject(m_pClient, &m_ProtocolCallContext, param)) {
        CompleteJob(m_PostResult ? m_PostResult : qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext, new StepSequenceJob::Step(&JobDataStorePostObject::StepWaitingLogicServerCompletePostObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStorePostObject<Traits>::StepWaitingLogicServerCompletePostObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (!m_PostResult) {
        CompleteJob(m_PostResult);
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    CompleteJob(qResult(RESULT_SUCCESS));
}

template <typename Traits>
nn::nex::JobDataStorePostObject<Traits>::JobDataStorePostObject()
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pClient(nullptr), m_pFileServerClient(nullptr), m_UsesHttps(true), m_BufferSize(0),
      m_ThreadPriority(0), m_MinBytesPerSecond(0), m_MinTimeout(0), m_pListener(nullptr), m_ChunkSize(0), m_pDataId(nullptr), m_pDataCopy(nullptr), m_pData(nullptr),
      m_PostResult(true), m_pHttpListener(nullptr)
{
    // nothing to do
}

template <typename Traits>
nn::nex::JobDataStorePostObject<Traits>::~JobDataStorePostObject()
{
    if (m_pHttpListener != nullptr) {
        delete m_pHttpListener;
    }
    if (m_pFileServerClient != nullptr) {
        delete m_pFileServerClient;
    }
    if (m_pDataCopy != nullptr) {
        MemoryManager::Free(m_pDataCopy);
    }
}

typedef JobDataStorePostObject<DataStoreApiVersionTraitsV0> JobDataStorePostObjectV0;
typedef JobDataStorePostObject<DataStoreApiVersionTraitsV1> JobDataStorePostObjectV1;
ASSERT_OFFSET(JobDataStorePostObjectV0, m_Param, 0x108);
ASSERT_OFFSET(JobDataStorePostObjectV0, m_ReqPostInfo, 0x190);
ASSERT_OFFSET(JobDataStorePostObjectV0, m_pHttpListener, 0x1DC);
ASSERT_OFFSET(JobDataStorePostObjectV1, m_ReqPostInfo, 0x1A8);
ASSERT_OFFSET(JobDataStorePostObjectV1, m_pHttpListener, 0x1FC);

// 0x007DDC00 (name is ours)
template nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::JobDataStorePostObject();
// 0x007DDD9C slot 0x00
// 0x007DDCE0 (deleting dtor)
template nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::~JobDataStorePostObject();
// 0x007DD2B8 slot 0x34
template nn::nex::qResult nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::Initialize(
    unsigned int, nn::nex::DataStoreLogicServerClientInterface*, const nn::nex::DataStorePreparePostParamV1&, unsigned int, bool, unsigned int, unsigned int, unsigned int,
    unsigned int*, nn::nex::DataStorePostObjectEventListener*, unsigned int, const unsigned char*, bool);
// 0x007DD4B0 | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::CompleteJob(const nn::nex::qResult&);
// 0x007DD860 (name is ours)
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepLogicServerPreparePostObject();
// 0x007DD548 (name is ours)
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepFileServerPostObject();
// 0x007DD974 | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepLogicServerCompletePostObject();
// 0x007DDB30 | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingLogicServerCompletePostObject();

// 0x007DE708 | fefates:bytes [tier B]
template nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::JobDataStorePostObject();
// 0x007DE8A4 slot 0x00
// 0x007DE7E8 (deleting dtor) | fefates:bytes
template nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::~JobDataStorePostObject();
// 0x007DDE48 slot 0x34
template nn::nex::qResult nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::Initialize(
    unsigned int, nn::nex::DataStoreLogicServerClientInterface*, const nn::nex::DataStorePreparePostParam&, unsigned int, bool, unsigned int, unsigned int, unsigned int,
    unsigned long long*, nn::nex::DataStorePostObjectEventListener*, unsigned int, const unsigned char*, bool);
// 0x007DDFA4 | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::CompleteJob(const nn::nex::qResult&);
// 0x007DE358 | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepLogicServerPreparePostObject();
// 0x007DE040 (name is ours)
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepFileServerPostObject();
// 0x007DE46C | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepLogicServerCompletePostObject();
// 0x007DE638 | fefates:bytes [tier B]
template void nn::nex::JobDataStorePostObject<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingLogicServerCompletePostObject();

} // namespace nex
} // namespace nn
