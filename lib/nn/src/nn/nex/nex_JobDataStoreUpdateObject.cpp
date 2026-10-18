#include "nn/nex/nex_JobDataStoreUpdateObject.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreCompleteUpdateParam.h"
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
// the timeout of a post without a minimum speed
const u32 NO_TIMEOUT = 0x7FFFFFFF;

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

// 0x003B3980 slot 0x34
nn::nex::qResult nn::nex::JobDataStoreUpdateObject::Initialize(unsigned int callContextId, nn::nex::DataStoreLogicServerClientInterface* pClient,
                                                              const nn::nex::DataStorePrepareUpdateParam& param, unsigned int threadPriority, bool usesHttps,
                                                              unsigned int bufferSize, unsigned int minBytesPerSecond, unsigned int minTimeout,
                                                              nn::nex::DataStorePostObjectEventListener* pListener, unsigned int chunkSize, const unsigned char* pData,
                                                              bool copiesData)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_pClient = pClient;
    m_pFileServerClient = new DataStoreFileServerClient();
    m_pListener = pListener;
    m_Param.m_StructureVersion = param.m_StructureVersion;
    m_Param.m_DataId = param.m_DataId;
    m_Param.m_Size = param.m_Size;
    m_Param.m_UpdatePassword = param.m_UpdatePassword;
    m_Param.m_ExtraData = param.m_ExtraData;
    m_ThreadPriority = threadPriority;
    m_UsesHttps = usesHttps;
    m_BufferSize = bufferSize;
    m_ChunkSize = chunkSize;
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
    SetStep(Step(&JobDataStoreUpdateObject::StepLogicServerPrepareUpdateObject, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

// 0x003B3B08 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = GetCallerContext(m_CallContextId);
    if (pContext != nullptr) {
        if (result) {
            pContext->SignalSuccess(result);
        } else {
            pContext->SignalFailure(result);
        }
    } else {
        m_ProtocolCallContext.Cancel(CallContext::STATE_CANCELLED);
    }
    SetToComplete();
}

// 0x003B3B88 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepFileServerPostObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        // (a message was logged: "Failed:PrepareUpdateObject 0x%x")
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    m_ProtocolCallContext.Reset();
    m_pHttpListener = new DataStorePostHttpEventListener(m_CallContextId, m_ReqUpdateInfo.m_RequestHeaders, m_ReqUpdateInfo.m_FormFields, m_ReqUpdateInfo.m_RootCaCert,
                                                         m_ThreadPriority, m_BufferSize, m_pDataCopy != nullptr ? m_pDataCopy : m_pData, m_Param.m_Size, m_pListener);
    String url;
    if (m_ReqUpdateInfo.m_Url.FindSubstringNoCase(L"http://") == 0 || m_ReqUpdateInfo.m_Url.FindSubstringNoCase(L"https://") == 0) {
        url = m_ReqUpdateInfo.m_Url;
    } else {
        url = (m_UsesHttps ? L"https://" : L"http://") + m_ReqUpdateInfo.m_Url;
    }
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
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobDataStoreUpdateObject::StepLogicServerCompleteUpdateObject, ""));
}

// 0x003B3EA4 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepLogicServerPrepareUpdateObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    if (!m_pClient->PrepareUpdateObject(&m_ProtocolCallContext, m_Param, &m_ReqUpdateInfo)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobDataStoreUpdateObject::StepFileServerPostObject, ""));
}

// 0x003B3FB8 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepLogicServerCompleteUpdateObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        m_PostResult = m_ProtocolCallContext.m_Result;
    }
    m_ProtocolCallContext.Reset();
    // the logic server gets the result of the post
    DataStoreCompleteUpdateParam param;
    param.m_DataId = m_Param.m_DataId;
    param.m_Version = m_ReqUpdateInfo.m_Version;
    param.m_IsSuccess = m_PostResult;
    if (!m_pClient->CompleteUpdateObject(&m_ProtocolCallContext, param)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    SetToSuspended();
    ResumeOnCallCompletion(&m_ProtocolCallContext, new Step(&JobDataStoreUpdateObject::StepWaitingLogicServerCompleteUpdateObject, ""));
}

// 0x003B4130 | fefates:bytes [tier B]
void nn::nex::JobDataStoreUpdateObject::StepWaitingLogicServerCompleteUpdateObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    if (m_PostResult) {
        CompleteJob(qResult(RESULT_SUCCESS));
    } else {
        CompleteJob(m_PostResult);
    }
}

// 0x003B4204 | mk7dlp:callseq [tier A]
nn::nex::JobDataStoreUpdateObject::JobDataStoreUpdateObject()
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pClient(nullptr), m_pFileServerClient(nullptr), m_UsesHttps(true), m_BufferSize(0),
      m_ThreadPriority(0), m_MinBytesPerSecond(0), m_MinTimeout(0), m_pListener(nullptr), m_ChunkSize(0), m_pDataCopy(nullptr), m_pData(nullptr),
      m_PostResult(RESULT_SUCCESS), m_pHttpListener(nullptr)
{
    // nothing to do
}

// 0x003B4378 slot 0x00 | fefates:bytes
// 0x003B4368 (deleting dtor)
nn::nex::JobDataStoreUpdateObject::~JobDataStoreUpdateObject()
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

} // namespace nex
} // namespace nn
