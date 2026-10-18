#include "nn/nex/nex_DataStoreClientTemplate.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_DataStoreLogicServerClient.h"
#include "nn/nex/nex_EndPoint.h"
#include "nn/nex/nex_JobDataStoreCheckNotification.h"
#include "nn/nex/nex_JobDataStoreGetObject.h"
#include "nn/nex/nex_JobDataStoreGetRating.h"
#include "nn/nex/nex_JobDataStorePostObject.h"
#include "nn/nex/nex_JobDataStoreReceiveNewArrivedObjects.h"
#include "nn/nex/nex_JobDataStoreUpdateObject.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the results (the names are ours)
const int RESULT_SUCCESS = qResult::SUCCESS;
const int RESULT_FAILURE = qResult::FAILURE;
const int RESULT_CALL_FAILED = static_cast<int>(0x8001000D);
// the defaults of the HTTP requests of the jobs
const u32 DEFAULT_THREAD_PRIORITY = 16;
const u32 DEFAULT_MIN_BYTES_PER_SECOND = 167;
const u32 DEFAULT_MIN_TIMEOUT = 60000;
const u32 DEFAULT_CHUNK_SIZE = 0x4000;
const u32 DEFAULT_HTTP_BUFFER_SIZE = 0x8000;

// (inline) the scheduler runs the job
inline void QueueJob(Job* pJob)
{
    Core* pCore = Core::GetInstance();
    (pCore != nullptr ? pCore->m_pScheduler : nullptr)->Queue(pJob, false);
}
} // namespace

template <typename T>
nn::nex::DataStoreClientTemplate<T>::DataStoreClientTemplate()
{
    m_ThreadPriority = DEFAULT_THREAD_PRIORITY;
    m_UsesHttps = true;
    m_MinBytesPerSecond = DEFAULT_MIN_BYTES_PER_SECOND;
    m_MinTimeout = DEFAULT_MIN_TIMEOUT;
    m_ChunkSize = DEFAULT_CHUNK_SIZE;
    m_HttpBufferSize = DEFAULT_HTTP_BUFFER_SIZE;
    // (a message was logged)
}

template <typename T>
nn::nex::DataStoreClientTemplate<T>::~DataStoreClientTemplate()
{
    // nothing to do
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::SetCallOwner(nn::nex::ProtocolCallContext* pContext)
{
    if (!this->UsesConnection()) {
        return true;
    }
    Credentials* pCredentials = pContext->m_pCredentials != nullptr ? pContext->m_pCredentials : this->m_pDefaultCredentials;
    if (pCredentials == nullptr) {
        return false;
    }
    EndPoint* pConnection = pCredentials->GetConnection(T::PROTOCOL_ID);
    if (pConnection == nullptr) {
        return false;
    }
    pContext->SetOwner(pConnection, pConnection->m_ConnectionId);
    return true;
}

template <typename T>
template <typename Traits>
bool nn::nex::DataStoreClientTemplate<T>::GetObjectImpl(nn::nex::ProtocolCallContext* pContext, const typename Traits::TPrepareGetParam& param,
                                                        nn::nex::DataStoreGetObjectEventListenerTemplate<typename Traits::TDataId>* pListener, unsigned char* pBuffer,
                                                        unsigned int bufferSize, bool getsMeta, unsigned int* pSize)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStoreGetObject<Traits>* pJob = new JobDataStoreGetObject<Traits>();
    qResult result = pJob->Initialize(pContext->m_Id, this, param, m_ThreadPriority, m_UsesHttps, m_HttpBufferSize, m_MinBytesPerSecond, m_MinTimeout, pListener,
                                      m_ChunkSize, pBuffer, bufferSize, getsMeta, pSize, nullptr);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pJob->ReleaseRef();
    return result;
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObjectImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, ListenerV1* pListener,
                                                        unsigned char* pBuffer, unsigned int bufferSize, unsigned int* pSize)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStoreGetObject<DataStoreApiVersionTraitsV1>* pJob = new JobDataStoreGetObject<DataStoreApiVersionTraitsV1>();
    DataStorePrepareGetParam param;
    qResult result = pJob->Initialize(pContext->m_Id, this, param, m_ThreadPriority, m_UsesHttps, m_HttpBufferSize, m_MinBytesPerSecond, m_MinTimeout, pListener,
                                      m_ChunkSize, pBuffer, bufferSize, false, pSize, &reqGetInfo);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pJob->ReleaseRef();
    return result;
}

template <typename T>
template <typename Traits>
bool nn::nex::DataStoreClientTemplate<T>::PostObjectImpl(nn::nex::ProtocolCallContext* pContext, const typename Traits::TPreparePostParam& param,
                                                         typename Traits::TPreparePostParam::TDataId* pDataId, nn::nex::DataStorePostObjectEventListener* pListener,
                                                         const unsigned char* pData, bool copiesData)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStorePostObject<Traits>* pJob = new JobDataStorePostObject<Traits>();
    qResult result = pJob->Initialize(pContext->m_Id, this, param, m_ThreadPriority, m_UsesHttps, m_HttpBufferSize, m_MinBytesPerSecond, m_MinTimeout, pDataId,
                                      pListener, m_ChunkSize, pData, copiesData);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pJob->ReleaseRef();
    return result;
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::UpdateObjectImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param,
                                                           nn::nex::DataStorePostObjectEventListener* pListener, const unsigned char* pData, bool copiesData)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStoreUpdateObject* pJob = new JobDataStoreUpdateObject();
    qResult result = pJob->Initialize(pContext->m_Id, this, param, m_ThreadPriority, m_UsesHttps, m_HttpBufferSize, m_MinBytesPerSecond, m_MinTimeout, pListener,
                                      m_ChunkSize, pData, copiesData);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pJob->ReleaseRef();
    return result;
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::CheckNotificationImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& url, nn::nex::String* pUrl,
                                                                unsigned long long lastDataId, unsigned long long* pLastDataId, unsigned int interval,
                                                                unsigned int* pInterval, unsigned int principalId, bool* pIsLogicServerError, bool getsUrl)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStoreCheckNotification* pJob = new JobDataStoreCheckNotification();
    qResult result = pJob->Initialize(pContext->m_Id, this, m_ThreadPriority, m_MinTimeout, url, pUrl, lastDataId, pLastDataId, interval, pInterval, principalId,
                                      pIsLogicServerError, getsUrl);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pJob->ReleaseRef();
    return result;
}

template <typename T>
template <typename Traits>
nn::nex::qResult nn::nex::DataStoreClientTemplate<T>::ReceiveNewArrivedObjectsImpl(
    nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
    nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<typename Traits::TDataId>*> listeners,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<Traits>* pEventListener, bool* pHasNext)
{
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return qResult(RESULT_FAILURE);
    }
    u32 principalId = this->GetPrincipalId();
    if (pContext->m_pCredentials != nullptr) {
        principalId = pContext->m_pCredentials->m_PrincipalId;
    }
    JobDataStoreReceiveNewArrivedObjects<Traits>* pJob = new JobDataStoreReceiveNewArrivedObjects<Traits>();
    qResult result = pJob->Initialize(pContext->m_Id, this, this, m_ThreadPriority, m_UsesHttps, principalId, pState, listeners, pEventListener, pHasNext);
    if (result) {
        QueueJob(pJob);
        return qResult(RESULT_SUCCESS);
    }
    pContext->SignalFailure(result);
    pJob->ReleaseRef();
    return result;
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRatingImpl(nn::nex::ProtocolCallContext* pContext, unsigned long long dataId,
                                                        nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings, unsigned long long accessPassword)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStoreGetRating* pJob = new JobDataStoreGetRating();
    qResult result = pJob->Initialize(pContext->m_Id, this, dataId, pRatings, accessPassword);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pContext->SignalFailure(result);
    pJob->ReleaseRef();
    return result;
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRatingsImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<unsigned long long>& dataIds,
                                                         nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings,
                                                         nn::nex::qVector<nn::nex::qResult>* pResults)
{
    if (!pContext->InitiateCall()) {
        return false;
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return false;
    }
    JobDataStoreGetRating* pJob = new JobDataStoreGetRating();
    qResult result = pJob->Initialize(pContext->m_Id, this, dataIds, pRatings, pResults);
    if (result) {
        QueueJob(pJob);
        return true;
    }
    pContext->SignalFailure(result);
    pJob->ReleaseRef();
    return result;
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target,
                                                    nn::nex::DataStoreRatingInfo* pRating, u64 accessPassword)
{
    return this->m_Protocol.CallGetRating(pContext, target, pRating, accessPassword);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                                                     nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings,
                                                     nn::nex::qVector<nn::nex::qResult>* pResults, u64 accessPassword)
{
    return this->m_Protocol.CallGetRatings(pContext, dataIds, pRatings, pResults, accessPassword);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, ListenerV1* pListener)
{
    return GetObjectImpl<DataStoreApiVersionTraitsV1>(pContext, param, pListener, nullptr, 0, false, nullptr);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param,
                                                                ListenerV1* pListener)
{
    return GetObjectImpl<DataStoreApiVersionTraitsV1>(pContext, param, pListener, nullptr, 0, true, nullptr);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, ListenerV0* pListener)
{
    return GetObjectImpl<DataStoreApiVersionTraitsV0>(pContext, param, pListener, nullptr, 0, false, nullptr);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, unsigned char* pBuffer,
                                                    unsigned int bufferSize, unsigned int* pSize)
{
    return GetObjectImpl<DataStoreApiVersionTraitsV1>(pContext, param, nullptr, pBuffer, bufferSize, false, pSize);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param,
                                                     unsigned long long* pDataId, nn::nex::DataStorePostObjectEventListener* pListener)
{
    return PostObjectImpl<DataStoreApiVersionTraitsV1>(pContext, param, pDataId, pListener, nullptr, false);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, unsigned int* pDataId,
                                                     nn::nex::DataStorePostObjectEventListener* pListener)
{
    return PostObjectImpl<DataStoreApiVersionTraitsV0>(pContext, param, pDataId, pListener, nullptr, false);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param,
                                                     unsigned long long* pDataId, const unsigned char* pData, bool copiesData)
{
    return PostObjectImpl<DataStoreApiVersionTraitsV1>(pContext, param, pDataId, nullptr, pData, copiesData);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostMetaBinaryDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId)
{
    return this->m_Protocol.CallPostMetaBinary(pContext, param, pDataId);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostMetaBinaryWithDataIdDirect(nn::nex::ProtocolCallContext* pContext, u64 dataId,
                                                                         const nn::nex::DataStorePreparePostParam& param)
{
    return this->m_Protocol.CallPostMetaBinaryWithDataId(pContext, dataId, param);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostMetaBinariesWithDataIdDirect(nn::nex::ProtocolCallContext* pContext,
                                                                           const nn::nex::qVector<std::pair<u64, nn::nex::DataStorePreparePostParam> >& params,
                                                                           bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return T::PostMetaBinariesWithDataId(pContext, params, transactional, pResults);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::PostMetaBinaryWithDataIdsDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                                                                          const nn::nex::DataStorePreparePostParam& param, bool transactional,
                                                                          nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStorePreparePostParam> params;
    params.push_back(param);
    return this->m_Protocol.CallPostMetaBinariesWithDataId(pContext, dataIds, params, transactional, pResults);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::UpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param,
                                                       nn::nex::DataStorePostObjectEventListener* pListener)
{
    return UpdateObjectImpl(pContext, param, pListener, nullptr, false);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::UpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param,
                                                       const unsigned char* pData, bool copiesData)
{
    return UpdateObjectImpl(pContext, param, nullptr, pData, copiesData);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::CheckNotification(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& url, nn::nex::String* pUrl,
                                                            unsigned long long lastDataId, unsigned long long* pLastDataId, unsigned int interval,
                                                            unsigned int* pInterval, unsigned int principalId, bool* pIsLogicServerError, bool getsUrl)
{
    return CheckNotificationImpl(pContext, url, pUrl, lastDataId, pLastDataId, interval, pInterval, principalId, pIsLogicServerError, getsUrl);
}

template <typename T>
nn::nex::qResult nn::nex::DataStoreClientTemplate<T>::CheckNewArrivedObjects(
    nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV1>* pEventListener, bool* pHasNewObjects)
{
    if (!pContext->InitiateCall()) {
        return qResult(RESULT_CALL_FAILED);
    }
    if (!SetCallOwner(pContext)) {
        pContext->SignalFailure(qResult(RESULT_FAILURE));
        return qResult(RESULT_FAILURE);
    }
    u32 principalId = this->GetPrincipalId();
    if (pContext->m_pCredentials != nullptr) {
        principalId = pContext->m_pCredentials->m_PrincipalId;
    }
    JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV1>* pJob = new JobDataStoreReceiveNewArrivedObjects<DataStoreApiVersionTraitsV1>();
    qResult result = pJob->Initialize(pContext->m_Id, this, m_ThreadPriority, principalId, pState, pEventListener, pHasNewObjects);
    if (result) {
        QueueJob(pJob);
        return qResult(RESULT_SUCCESS);
    }
    pContext->SignalFailure(result);
    pJob->ReleaseRef();
    return result;
}

template <typename T>
nn::nex::qResult nn::nex::DataStoreClientTemplate<T>::ReceiveNewArrivedObjects(
    nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState, nn::nex::qVector<ListenerV1*> listeners,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV1>* pEventListener, bool* pHasNext)
{
    return ReceiveNewArrivedObjectsImpl<DataStoreApiVersionTraitsV1>(pContext, pState, listeners, pEventListener, pHasNext);
}

template <typename T>
nn::nex::qResult nn::nex::DataStoreClientTemplate<T>::ReceiveNewArrivedObjects(
    nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState, nn::nex::qVector<ListenerV0*> listeners,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV0>* pEventListener, bool* pHasNext)
{
    return ReceiveNewArrivedObjectsImpl<DataStoreApiVersionTraitsV0>(pContext, pState, listeners, pEventListener, pHasNext);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRating(nn::nex::ProtocolCallContext* pContext, unsigned long long dataId,
                                                    nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings, unsigned long long accessPassword)
{
    return GetRatingImpl(pContext, dataId, pRatings, accessPassword);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<unsigned long long>& dataIds,
                                                     nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings,
                                                     nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return GetRatingsImpl(pContext, dataIds, pRatings, pResults);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetRatingWithLogDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target,
                                                                 nn::nex::DataStoreRatingInfo* pRating, nn::nex::DataStoreRatingLog* pRatingLog, u64 accessPassword)
{
    return this->m_Protocol.CallGetRatingWithLog(pContext, target, pRating, pRatingLog, accessPassword);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, ListenerV1* pListener)
{
    return GetObjectImpl(pContext, reqGetInfo, pListener, nullptr, 0, nullptr);
}

template <typename T>
bool nn::nex::DataStoreClientTemplate<T>::GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, unsigned char* pBuffer,
                                                    unsigned int bufferSize, unsigned int* pSize)
{
    return GetObjectImpl(pContext, reqGetInfo, nullptr, pBuffer, bufferSize, pSize);
}

typedef DataStoreClientTemplate<DataStoreLogicServerClient> DataStoreClient;
ASSERT_OFFSET(DataStoreClient, m_ThreadPriority, 0x60);
ASSERT_OFFSET(DataStoreClient, m_HttpBufferSize, 0x74);
ASSERT_SIZE(DataStoreClient, 0x78);

// 0x007E01A0 | fefates:callseq
template nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::DataStoreClientTemplate();
// 0x007E02F0 slot 0x00
// 0x007E0278 (deleting dtor)
// 0x00774DFC (thunk 16)
// 0x00774D80 (deleting thunk 16)
// 0x00779550 (thunk 92)
// 0x007794D4 (deleting thunk 92)
template nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::~DataStoreClientTemplate();
// 0x00827C68 slot 0x24
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::SetCallOwner(nn::nex::ProtocolCallContext*);
// 0x007E00F8 slot 0xB8
// 0x00774D18 (thunk 16)
// 0x00779424 (thunk 92)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRating(nn::nex::ProtocolCallContext*, const nn::nex::DataStoreRatingTarget&,
                                                                                               nn::nex::DataStoreRatingInfo*, unsigned long long);
// 0x007E013C slot 0xBC
// 0x00774D48 (thunk 16)
// 0x0077946C (thunk 92)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRatings(
    nn::nex::ProtocolCallContext*, const nn::nex::qVector<unsigned long long>&, nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >*,
    nn::nex::qVector<nn::nex::qResult>*, unsigned long long);
// 0x007E0064 slot 0x10C
// 0x00779388 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStorePrepareGetParam&,
                                                                                               nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*);
// 0x007DFB40 slot 0x110
// 0x00779288 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObjectOrMetaBinary(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStorePrepareGetParam&, nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*);
// 0x007DEA68 slot 0x114
// 0x0077916C (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStorePrepareGetParamV1&,
                                                                                               nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*);
// 0x007E008C slot 0x118
// 0x007793B4 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStorePrepareGetParam&,
                                                                                               unsigned char*, unsigned int, unsigned int*);
// 0x007DE984 slot 0x11C
// 0x00779100 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStorePreparePostParam&,
                                                                                                unsigned long long*, nn::nex::DataStorePostObjectEventListener*);
// 0x007DEA90 slot 0x120
// 0x00779198 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostObject(nn::nex::ProtocolCallContext*,
                                                                                                const nn::nex::DataStorePreparePostParamV1&, unsigned int*,
                                                                                                nn::nex::DataStorePostObjectEventListener*);
// 0x007DE958 slot 0x124
// 0x007790D0 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStorePreparePostParam&,
                                                                                                unsigned long long*, const unsigned char*, bool);
// 0x007DE950 slot 0x128
// 0x007790C8 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostMetaBinaryDirect(nn::nex::ProtocolCallContext*,
                                                                                                          const nn::nex::DataStorePreparePostParam&, unsigned long long*);
// 0x007DEA40 slot 0x12C
// 0x00779144 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostMetaBinaryWithDataIdDirect(nn::nex::ProtocolCallContext*, unsigned long long,
                                                                                                                    const nn::nex::DataStorePreparePostParam&);
// 0x007DE9AC slot 0x130
// 0x0077912C (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostMetaBinariesWithDataIdDirect(
    nn::nex::ProtocolCallContext*, const nn::nex::qVector<std::pair<unsigned long long, nn::nex::DataStorePreparePostParam> >&, bool,
    nn::nex::qVector<nn::nex::qResult>*);
// 0x007DE9C0 slot 0x134
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostMetaBinaryWithDataIdsDirect(
    nn::nex::ProtocolCallContext*, const nn::nex::qVector<unsigned long long>&, const nn::nex::DataStorePreparePostParam&, bool, nn::nex::qVector<nn::nex::qResult>*);
// 0x007DEAD8 slot 0x138
// 0x007791E8 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::UpdateObject(nn::nex::ProtocolCallContext*,
                                                                                                  const nn::nex::DataStorePrepareUpdateParam&,
                                                                                                  nn::nex::DataStorePostObjectEventListener*);
// 0x007DEAB8 slot 0x13C
// 0x007791C4 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::UpdateObject(nn::nex::ProtocolCallContext*,
                                                                                                  const nn::nex::DataStorePrepareUpdateParam&, const unsigned char*, bool);
// 0x007DF7FC slot 0x140
// 0x00779264 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::CheckNotification(nn::nex::ProtocolCallContext*, const nn::nex::String&,
                                                                                                       nn::nex::String*, unsigned long long, unsigned long long*,
                                                                                                       unsigned int, unsigned int*, unsigned int, bool*, bool);
// 0x007DF444 slot 0x144
template nn::nex::qResult nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::CheckNewArrivedObjects(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStoreNewArrivedObjectsState*,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >*, bool*);
// 0x007DFB6C slot 0x148
// 0x007792B8 (thunk)
template nn::nex::qResult nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::ReceiveNewArrivedObjects(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStoreNewArrivedObjectsState*, nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*>,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >*, bool*);
// 0x007DFBD0 slot 0x14C
// 0x00779320 (thunk)
template nn::nex::qResult nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::ReceiveNewArrivedObjects(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStoreNewArrivedObjectsState*, nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*>,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >*, bool*);
// 0x007E0174 slot 0x150
// 0x007794A4 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRating(nn::nex::ProtocolCallContext*, unsigned long long,
                                                                                               nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>*,
                                                                                               unsigned long long);
// 0x007E0128 slot 0x154
// 0x00779454 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRatings(
    nn::nex::ProtocolCallContext*, const nn::nex::qVector<unsigned long long>&, nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >*,
    nn::nex::qVector<nn::nex::qResult>*);
// 0x007E00C0 slot 0x158
// 0x007793EC (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRatingWithLogDirect(nn::nex::ProtocolCallContext*,
                                                                                                            const nn::nex::DataStoreRatingTarget&,
                                                                                                            nn::nex::DataStoreRatingInfo*, nn::nex::DataStoreRatingLog*,
                                                                                                            unsigned long long);
// 0x007DF628 slot 0x15C
// 0x0077920C (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStoreReqGetInfo&,
                                                                                               nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*);
// 0x007DF64C slot 0x160
// 0x00779234 (thunk)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObject(nn::nex::ProtocolCallContext*, const nn::nex::DataStoreReqGetInfo&,
                                                                                               unsigned char*, unsigned int, unsigned int*);
// 0x007DEAF8 (name is ours)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObjectImpl<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStorePrepareGetParamV1&, nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*, unsigned char*,
    unsigned int, bool, unsigned int*);
// 0x007DEC98 (name is ours)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObjectImpl<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStorePrepareGetParam&, nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*, unsigned char*,
    unsigned int, bool, unsigned int*);
// 0x007DF81C (name is ours)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetObjectImpl(nn::nex::ProtocolCallContext*, const nn::nex::DataStoreReqGetInfo&,
                                                                                                   nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*,
                                                                                                   unsigned char*, unsigned int, unsigned int*);
// 0x007DF11C (name is ours)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostObjectImpl<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStorePreparePostParamV1&, unsigned int*, nn::nex::DataStorePostObjectEventListener*, const unsigned char*, bool);
// 0x007DF2B0 | fefates:callseq
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::PostObjectImpl<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStorePreparePostParam&, unsigned long long*, nn::nex::DataStorePostObjectEventListener*, const unsigned char*, bool);
// 0x007DF678 | fefates:callseq
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::UpdateObjectImpl(nn::nex::ProtocolCallContext*,
                                                                                                      const nn::nex::DataStorePrepareUpdateParam&,
                                                                                                      nn::nex::DataStorePostObjectEventListener*, const unsigned char*, bool);
// 0x007DF9D8 | fefates:callseq
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::CheckNotificationImpl(nn::nex::ProtocolCallContext*, const nn::nex::String&,
                                                                                                           nn::nex::String*, unsigned long long, unsigned long long*,
                                                                                                           unsigned int, unsigned int*, unsigned int, bool*, bool);
// 0x007DFC34 (name is ours)
template nn::nex::qResult
nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::ReceiveNewArrivedObjectsImpl<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStoreNewArrivedObjectsState*, nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned int>*>,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)0> >*, bool*);
// 0x007DFE4C (name is ours)
template nn::nex::qResult
nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::ReceiveNewArrivedObjectsImpl<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >(
    nn::nex::ProtocolCallContext*, const nn::nex::DataStoreNewArrivedObjectsState*, nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<unsigned long long>*>,
    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraits<(nn::nex::DATASTORE_API_VERSION)1> >*, bool*);
// 0x007DEFAC (name is ours)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRatingImpl(nn::nex::ProtocolCallContext*, unsigned long long,
                                                                                                   nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>*,
                                                                                                   unsigned long long);
// 0x007DEE38 (name is ours)
template bool nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>::GetRatingsImpl(
    nn::nex::ProtocolCallContext*, const nn::nex::qVector<unsigned long long>&, nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >*,
    nn::nex::qVector<nn::nex::qResult>*);

} // namespace nex
} // namespace nn
