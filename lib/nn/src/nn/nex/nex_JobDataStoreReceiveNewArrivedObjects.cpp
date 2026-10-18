#include "nn/nex/nex_JobDataStoreReceiveNewArrivedObjects.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_DataStoreGetNewArrivedNotificationsParam.h"
#include "nn/nex/nex_DataStoreLogicServerClientInterface.h"
#include "nn/nex/nex_MD5Checksum.h"
#include "nn/nex/nex_Scheduler.h"

#include <cstring>

namespace nn {
namespace nex {
// 0x0095F5FC (name is ours)
template <>
JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV0>::GetObjectMethod JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV0>::s_GetObject =
    &DataStoreClientInterface::GetObject;
// 0x0095F604 (name is ours)
template <>
JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV1>::GetObjectMethod JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV1>::s_GetObject =
    &DataStoreClientInterface::GetObject;

namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
const int RESULT_INVALID_SIZE = static_cast<int>(0x80690001);
const int RESULT_INVALID_STATE = static_cast<int>(0x80690008);
// the module of the results of DataStore
const u32 MODULE_DATASTORE = 105;
// the failed checks after which the logic server gives a new URL
const u32 MAX_ERROR_COUNT = 10;
// the bytes of the state in its checksum
const u32 STATE_CHECKSUM_SIZE = 344;

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

// (inline) the MD5 checksum of the state
inline u32 ComputeChecksum(const DataStoreNewArrivedObjectsState* pState)
{
    MD5Checksum md5;
    const u8* pData = reinterpret_cast<const u8*>(pState);
    const u32 sizes[] = {STATE_CHECKSUM_SIZE};
    return md5.ComputeChecksumForTransportArray(&pData, sizes, 1);
}

// (inline) a new checksum of the state
inline void UpdateChecksum(DataStoreNewArrivedObjectsState* pState)
{
    pState->m_Checksum = ComputeChecksum(pState);
}

// (inline) the state of the caller is unchanged and has a URL
inline bool IsValid(const DataStoreNewArrivedObjectsState* pState)
{
    return ComputeChecksum(pState) == pState->m_Checksum && memchr(pState->m_Url, 0, sizeof(pState->m_Url)) != nullptr && strlen(pState->m_Url) != 0;
}

// (inline) the URL of the notification file in the state
inline void SetUrl(DataStoreNewArrivedObjectsState* pState, const String& url)
{
    memset(pState->m_Url, 0, sizeof(pState->m_Url));
    char* pUrl = nullptr;
    url.CreateCopy(&pUrl);
    strcpy(pState->m_Url, pUrl);
    String::ReleaseCopy(pUrl);
    UpdateChecksum(pState);
}

// (inline) GetNewArrivedNotifications of the version of the notifications
inline bool GetNewArrivedNotifications(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStoreGetNewArrivedNotificationsParam& param,
                                       qVector<DataStoreNotificationV1>* pResult, bool* pHasNext)
{
    return pClient->GetNewArrivedNotificationsV1(pContext, param, pResult, pHasNext);
}
inline bool GetNewArrivedNotifications(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStoreGetNewArrivedNotificationsParam& param,
                                       qVector<DataStoreNotification>* pResult, bool* pHasNext)
{
    return pClient->GetNewArrivedNotifications(pContext, param, pResult, pHasNext);
}

// (inline) GetSpecificMeta of the version of the parameters
inline bool GetSpecificMeta(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStoreGetSpecificMetaParamV1& param,
                            qVector<DataStoreSpecificMetaInfoV1>* pMetaInfos)
{
    return pClient->GetSpecificMetaV1(pContext, param, pMetaInfos);
}
inline bool GetSpecificMeta(DataStoreLogicServerClientInterface* pClient, ProtocolCallContext* pContext, const DataStoreGetSpecificMetaParam& param,
                            qVector<DataStoreSpecificMetaInfo>* pMetaInfos)
{
    return pClient->GetSpecificMeta(pContext, param, pMetaInfos);
}

// (inline) a new state of the caller
inline void InitializeState(DataStoreNewArrivedObjectsState* pState)
{
    pState->m_Unknown0x00 = 0;
    pState->m_ErrorCount = 0;
    pState->m_Interval = 0;
    pState->m_Unknown0x0C = 0;
    pState->m_LastNotificationId = 0;
    pState->m_Unknown0x15C = 0;
    memset(pState->m_Url, 0, sizeof(pState->m_Url));
    UpdateChecksum(pState);
}
} // namespace

template <typename Traits>
nn::nex::qResult nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::Initialize(unsigned int callContextId, nn::nex::DataStoreClientInterface* pClient,
                                                                                  nn::nex::DataStoreLogicServerClientInterface* pLogicClient, unsigned int threadPriority,
                                                                                  bool usesHttps, unsigned int principalId,
                                                                                  const nn::nex::DataStoreNewArrivedObjectsState* pState, nn::nex::qVector<Listener*> listeners,
                                                                                  EventListener* pEventListener, bool* pHasNext)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    m_pClient = pClient;
    m_pLogicClient = pLogicClient;
    m_ThreadPriority = threadPriority;
    m_UsesHttps = usesHttps;
    m_PrincipalId = principalId;
    m_Listeners = listeners;
    m_pEventListener = pEventListener;
    *pHasNext = false;
    m_pHasNext = pHasNext;
    m_ChecksOnly = false;
    if (pState != nullptr) {
        if (!IsValid(pState)) {
            return qResult(RESULT_INVALID_STATE);
        }
        m_State = *pState;
    } else {
        InitializeState(&m_State);
    }
    m_NewState = m_State;
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepCheckNotification, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

template <typename Traits>
nn::nex::qResult nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::Initialize(unsigned int callContextId, nn::nex::DataStoreClientInterface* pClient,
                                                                                  unsigned int threadPriority, unsigned int principalId,
                                                                                  const nn::nex::DataStoreNewArrivedObjectsState* pState, EventListener* pEventListener,
                                                                                  bool* pHasNewObjects)
{
    ScopedCS lock(GetSchedulerLock());
    m_pLogicClient = nullptr;
    m_CallContextId = callContextId;
    m_pClient = pClient;
    m_ThreadPriority = threadPriority;
    m_UsesHttps = true;
    m_PrincipalId = principalId;
    m_pEventListener = pEventListener;
    m_pHasNext = nullptr;
    m_ChecksOnly = true;
    m_pHasNewObjects = pHasNewObjects;
    *pHasNewObjects = false;
    m_IsNewState = pState == nullptr;
    if (m_IsNewState) {
        InitializeState(&m_State);
    } else {
        if (!IsValid(pState)) {
            return qResult(RESULT_INVALID_STATE);
        }
        m_State = *pState;
    }
    m_NewState = m_State;
    m_ProtocolCallContext.SetCredentials(static_cast<ProtocolCallContext*>(GetCallerContext(m_CallContextId))->m_pCredentials);
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepCheckNotification, ""));
    bool isSuccess = true;
    return qResult(isSuccess);
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::CompleteJob(const nn::nex::qResult& result)
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
    Job::SetToComplete();
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepCheckNotification()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    m_ProtocolCallContext.Reset();
    bool isCalled;
    {
        String url(m_State.m_Url);
        isCalled = m_pClient->CheckNotification(&m_ProtocolCallContext, url, &m_Url, m_State.m_LastNotificationId, &m_LastDataId, m_State.m_Interval, &m_Interval,
                                                m_PrincipalId, &m_IsLogicServerError, m_State.m_ErrorCount >= MAX_ERROR_COUNT);
    }
    if (!isCalled) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext,
                                            new StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepWaitingCheckNotification, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepWaitingCheckNotification()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    bool isChanged = false;
    bool isUpToDate = false;
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        if (m_IsLogicServerError) {
            CompleteJob(m_ProtocolCallContext.m_Result);
            return;
        }
        m_NewState.m_ErrorCount++;
        UpdateChecksum(&m_NewState);
        // (a message was logged: "Failed: CheckNotification 0x%x, errorCount: %u")
        isChanged = true;
        isUpToDate = true;
    } else if (String::IsEqual(m_Url.m_pString, L"invalidurl")) {
        // no notification file for the principal: the state of the server
        m_NewState.m_LastNotificationId = m_LastDataId;
        UpdateChecksum(&m_NewState);
        SetUrl(&m_NewState, m_Url);
        m_NewState.m_Interval = m_Interval;
        UpdateChecksum(&m_NewState);
        m_pEventListener->UpdateState(&m_NewState, nullptr, m_NewState.m_LastNotificationId);
        CompleteJob(qResult(RESULT_SUCCESS));
        return;
    } else {
        if (m_NewState.m_ErrorCount != 0) {
            m_NewState.m_ErrorCount = 0;
            UpdateChecksum(&m_NewState);
            isChanged = true;
        }
        if (!String::IsEqual(String(m_State.m_Url).m_pString, m_Url.m_pString)) {
            SetUrl(&m_NewState, m_Url);
            isChanged = true;
        }
        if (m_Interval != m_State.m_Interval) {
            m_NewState.m_Interval = m_Interval;
            UpdateChecksum(&m_NewState);
            isChanged = true;
        }
        if (m_State.m_LastNotificationId >= m_LastDataId) {
            isUpToDate = true;
        }
    }
    if (m_ChecksOnly && (!isUpToDate || m_IsNewState)) {
        *m_pHasNewObjects = true;
    }
    if (isChanged) {
        m_pEventListener->UpdateState(&m_NewState, nullptr, m_NewState.m_LastNotificationId);
    }
    if (isUpToDate || m_ChecksOnly) {
        CompleteJob(qResult(RESULT_SUCCESS));
        return;
    }
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepGetNewArrivedNotifications, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepGetNewArrivedNotifications()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    DataStoreGetNewArrivedNotificationsParam param;
    param.m_LastNotificationId = m_State.m_LastNotificationId;
    param.m_Limit = static_cast<u16>(m_Listeners.size());
    if (!GetNewArrivedNotifications(m_pLogicClient, &m_ProtocolCallContext, param, &m_Notifications, m_pHasNext)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext,
                                            new StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepWaitingGetNewArrivedNotifications, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepWaitingGetNewArrivedNotifications()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    for (TNotification* p = m_Notifications.begin(); p != m_Notifications.end(); p++) {
        // (a message was logged for each notification)
    }
    if (m_Notifications.begin() == m_Notifications.end()) {
        m_NewState.m_LastNotificationId = m_LastDataId;
        UpdateChecksum(&m_NewState);
        m_pEventListener->UpdateState(&m_NewState, nullptr, m_NewState.m_LastNotificationId);
        CompleteJob(qResult(RESULT_SUCCESS));
    }
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepGetSpecificMeta, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepGetSpecificMeta()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    TGetSpecificMetaParam param;
    for (TNotification* p = m_Notifications.begin(); p != m_Notifications.end(); p++) {
        if (p->m_DataId != 0) {
            param.m_DataIds.push_back(p->m_DataId);
        }
    }
    if (param.m_DataIds.begin() == param.m_DataIds.end()) {
        StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepGetObject, ""));
        return;
    }
    if (!GetSpecificMeta(m_pLogicClient, &m_ProtocolCallContext, param, &m_MetaInfos)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext,
                                            new StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepWaitingGetSpecificMeta, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepWaitingGetSpecificMeta()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        CompleteJob(m_ProtocolCallContext.m_Result);
        return;
    }
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepGetObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepGetObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_ListenerIndex > m_Listeners.size() || m_Notifications.size() == m_NotificationIndex) {
        CompleteJob(qResult(RESULT_SUCCESS));
        return;
    }
    TSpecificMetaInfo metaInfo;
    bool isFound = false;
    for (TSpecificMetaInfo* p = m_MetaInfos.begin(); p != m_MetaInfos.end(); p++) {
        if (p->m_DataId == m_Notifications.at(m_NotificationIndex).m_DataId) {
            metaInfo = *p;
            isFound = true;
        }
    }
    if (!isFound) {
        // no object (it was deleted): only the state
        m_NewState.m_LastNotificationId = m_Notifications.at(m_NotificationIndex).m_NotificationId;
        UpdateChecksum(&m_NewState);
        m_pEventListener->UpdateState(&m_NewState, nullptr, m_NewState.m_LastNotificationId);
        if (GetCallerContext(m_CallContextId) == nullptr) {
            CompleteJob(qResult(RESULT_CANCELLED));
            return;
        }
        m_NotificationIndex++;
        return;
    }
    m_pEventListener->BeginObject(metaInfo.m_DataId, metaInfo.m_Size, metaInfo.m_OwnerId, metaInfo.m_DataType, metaInfo.m_Version,
                                  m_Notifications.at(m_NotificationIndex).m_NotificationId);
    if (GetCallerContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (m_pAdaptor != nullptr) {
        delete m_pAdaptor;
    }
    m_pAdaptor = new Adaptor(m_Listeners.at(m_ListenerIndex), &m_ProtocolCallContext, metaInfo);
    TPrepareGetParam param;
    param.m_DataId = m_Notifications.at(m_NotificationIndex).m_DataId;
    if (!(m_pClient->*s_GetObject)(&m_ProtocolCallContext, param, m_pAdaptor)) {
        CompleteJob(qResult(RESULT_CALL_FAILED));
        return;
    }
    Core::GetInstance()->m_pCallContextRegister->AssociateCallContext(m_CallContextId, m_ProtocolCallContext.m_Id);
    Job::SetToSuspended();
    StepSequenceJob::ResumeOnCallCompletion(&m_ProtocolCallContext, new StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepWaitingGetObject, ""));
}

template <typename Traits>
void nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::StepWaitingGetObject()
{
    if (GetCallerContext(m_CallContextId) == nullptr || m_ProtocolCallContext.m_State == CallContext::STATE_CANCELLED) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    bool isSkipped = false;
    if (m_ProtocolCallContext.m_State != CallContext::STATE_SUCCESS) {
        qResult result = m_ProtocolCallContext.m_Result;
        if (result.GetModule() != MODULE_DATASTORE || result.Equals(RESULT_INVALID_SIZE)) {
            CompleteJob(result);
            return;
        }
        // another error of DataStore: the next object
        isSkipped = true;
    }
    m_NewState.m_LastNotificationId = m_Notifications.at(m_NotificationIndex).m_NotificationId;
    UpdateChecksum(&m_NewState);
    m_pEventListener->UpdateState(&m_NewState, isSkipped ? nullptr : m_Listeners.at(m_ListenerIndex), m_NewState.m_LastNotificationId);
    if (!isSkipped) {
        m_ListenerIndex++;
    }
    m_NotificationIndex++;
    StepSequenceJob::SetStep(StepSequenceJob::Step(&JobDataStoreReceiveNewArrivedObjects::StepGetObject, ""));
}

template <typename Traits>
nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::JobDataStoreReceiveNewArrivedObjects()
    : StepSequenceJob(static_cast<JobType>(2), DebugString()), m_CallContextId(0), m_pClient(nullptr), m_pLogicClient(nullptr), m_UsesHttps(true), m_ThreadPriority(0),
      m_LastDataId(0), m_Url(L""), m_Interval(0), m_PrincipalId(0), m_pEventListener(nullptr), m_ListenerIndex(0), m_NotificationIndex(0), m_pHasNext(nullptr),
      m_IsLogicServerError(false), m_pAdaptor(nullptr), m_ChecksOnly(false), m_pHasNewObjects(nullptr), m_IsNewState(false)
{
    memset(&m_State, 0, sizeof(m_State));
    memset(&m_NewState, 0, sizeof(m_NewState));
}

template <typename Traits>
nn::nex::JobDataStoreReceiveNewArrivedObjects<Traits>::~JobDataStoreReceiveNewArrivedObjects()
{
    if (m_pAdaptor != nullptr) {
        delete m_pAdaptor;
    }
}

typedef JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV0> JobDataStoreReceiveNewArrivedObjectsV0;
typedef JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV1> JobDataStoreReceiveNewArrivedObjectsV1;
ASSERT_OFFSET(JobDataStoreReceiveNewArrivedObjectsV0, m_ProtocolCallContext, 0x80);
ASSERT_OFFSET(JobDataStoreReceiveNewArrivedObjectsV0, m_Notifications, 0x110);
ASSERT_OFFSET(JobDataStoreReceiveNewArrivedObjectsV0, m_NewState, 0x280);
ASSERT_OFFSET(JobDataStoreReceiveNewArrivedObjectsV0, m_MetaInfos, 0x3FC);
ASSERT_OFFSET(JobDataStoreReceiveNewArrivedObjectsV0, m_IsNewState, 0x418);
ASSERT_SIZE(JobDataStoreReceiveNewArrivedObjectsV0, 0x420);
ASSERT_SIZE(JobDataStoreReceiveNewArrivedObjectsV1, 0x420);

// 0x007E201C
template nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::JobDataStoreReceiveNewArrivedObjects();
// 0x007E2108 slot 0x00
// 0x007E20F8 (deleting dtor)
template nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::~JobDataStoreReceiveNewArrivedObjects();
// 0x007E035C slot 0x34
template nn::nex::qResult nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::Initialize(
    unsigned int, nn::nex::DataStoreClientInterface*, nn::nex::DataStoreLogicServerClientInterface*, unsigned int, bool, unsigned int,
    const nn::nex::DataStoreNewArrivedObjectsState*, nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*>,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >*, bool*);
// 0x007E0858 slot 0x38
template nn::nex::qResult nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::Initialize(
    unsigned int, nn::nex::DataStoreClientInterface*, unsigned int, unsigned int, const nn::nex::DataStoreNewArrivedObjectsState*,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >*, bool*);
// 0x007E0AD4 | fefates:callgraph
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::CompleteJob(const nn::nex::qResult&);
// 0x007E1634 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepCheckNotification();
// 0x007E1884 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingCheckNotification();
// 0x007E1D54 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepGetNewArrivedNotifications();
// 0x007E1EA8 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingGetNewArrivedNotifications();
// 0x007E1094 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepGetSpecificMeta();
// 0x007E17C8 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingGetSpecificMeta();
// 0x007E0B54 | fefates:bytes [tier B]
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepGetObject();
// 0x007E13FC (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >::StepWaitingGetObject();

// 0x007E3D94
template nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::JobDataStoreReceiveNewArrivedObjects();
// 0x007E3F40 slot 0x00
// 0x007E3E70 (deleting dtor)
template nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::~JobDataStoreReceiveNewArrivedObjects();
// 0x007E21CC slot 0x34
template nn::nex::qResult nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::Initialize(
    unsigned int, nn::nex::DataStoreClientInterface*, nn::nex::DataStoreLogicServerClientInterface*, unsigned int, bool, unsigned int,
    const nn::nex::DataStoreNewArrivedObjectsState*, nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*>,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >*, bool*);
// 0x007E26C8 slot 0x38
template nn::nex::qResult nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::Initialize(
    unsigned int, nn::nex::DataStoreClientInterface*, unsigned int, unsigned int, const nn::nex::DataStoreNewArrivedObjectsState*,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >*, bool*);
// 0x007E2944 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::CompleteJob(const nn::nex::qResult&);
// 0x007E33AC (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepCheckNotification();
// 0x007E35FC (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingCheckNotification();
// 0x007E3ACC (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepGetNewArrivedNotifications();
// 0x007E3C20 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingGetNewArrivedNotifications();
// 0x007E2F44 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepGetSpecificMeta();
// 0x007E3540 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingGetSpecificMeta();
// 0x007E29C4 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepGetObject();
// 0x007E3174 (name is ours)
template void nn::nex::JobDataStoreReceiveNewArrivedObjects<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >::StepWaitingGetObject();

// 0x003D4528 (name is ours)
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned int>*>::qVector(const qVector<DataStoreGetObjectEventListenerTemplate<unsigned int>*>& other)
{
    InitializeCopyInline(other);
}

// 0x003D45C0 (name is ours)
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned int>*>::~qVector()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

// 0x003D4618 (name is ours)
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned long long>*>::qVector(
    const qVector<DataStoreGetObjectEventListenerTemplate<unsigned long long>*>& other)
{
    InitializeCopyInline(other);
}

// 0x003D46B0 (name is ours)
template <>
DECOMP_NOINLINE qVector<DataStoreGetObjectEventListenerTemplate<unsigned long long>*>::~qVector()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

} // namespace nex
} // namespace nn
