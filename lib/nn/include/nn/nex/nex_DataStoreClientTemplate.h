#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_DataStoreClientInterface.h"
#include "nn/nex/nex_DataStoreNewArrivedObjects.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class DataStorePostObjectEventListener;
class DataStorePrepareUpdateParam;
class ProtocolCallContext;
class String;

// The DataStore client of the application: the logic server client T with the API of
// DataStoreClientInterface (the jobs of the objects, ratings and notifications get the settings of
// the HTTP requests from here). The names of the members, of the Impl functions without a reference
// symbol and of the methods are ours.
// Instantiations found in the binary:
//   nn::nex::DataStoreClientTemplate<nn::nex::DataStoreLogicServerClient>  typeinfo 0x008CECB0  vtable 0x008FE22C
template <typename T>
class DataStoreClientTemplate : public T, public ::nn::nex::DataStoreClientInterface
{
public:
    typedef DataStoreClientInterface::ListenerV0 ListenerV0;
    typedef DataStoreClientInterface::ListenerV1 ListenerV1;

    DataStoreClientTemplate();
    virtual ~DataStoreClientTemplate();
    // the call is made through the connection of the protocol of T
    virtual bool SetCallOwner(nn::nex::ProtocolCallContext* pContext);
    virtual bool GetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating,
                           u64 accessPassword);
    virtual bool GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                            nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults,
                            u64 accessPassword);
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, ListenerV1* pListener);
    virtual bool GetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, ListenerV1* pListener);
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, ListenerV0* pListener);
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, unsigned char* pBuffer, unsigned int bufferSize,
                           unsigned int* pSize);
    virtual bool PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, unsigned long long* pDataId,
                            nn::nex::DataStorePostObjectEventListener* pListener);
    virtual bool PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, unsigned int* pDataId,
                            nn::nex::DataStorePostObjectEventListener* pListener);
    virtual bool PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, unsigned long long* pDataId,
                            const unsigned char* pData, bool copiesData);
    virtual bool PostMetaBinaryDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId);
    virtual bool PostMetaBinaryWithDataIdDirect(nn::nex::ProtocolCallContext* pContext, u64 dataId, const nn::nex::DataStorePreparePostParam& param);
    virtual bool PostMetaBinariesWithDataIdDirect(nn::nex::ProtocolCallContext* pContext,
                                                  const nn::nex::qVector<std::pair<u64, nn::nex::DataStorePreparePostParam> >& params, bool transactional,
                                                  nn::nex::qVector<nn::nex::qResult>* pResults);
    // the meta binary of the parameters with each of the data IDs
    virtual bool PostMetaBinaryWithDataIdsDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                                                 const nn::nex::DataStorePreparePostParam& param, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults);
    virtual bool UpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param,
                              nn::nex::DataStorePostObjectEventListener* pListener);
    virtual bool UpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, const unsigned char* pData, bool copiesData);
    virtual bool CheckNotification(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& url, nn::nex::String* pUrl, unsigned long long lastDataId,
                                   unsigned long long* pLastDataId, unsigned int interval, unsigned int* pInterval, unsigned int principalId,
                                   bool* pIsLogicServerError, bool getsUrl);
    // only the check of the notification file of the principal (are there new objects)
    virtual nn::nex::qResult CheckNewArrivedObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                                    nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV1>* pEventListener,
                                                    bool* pHasNewObjects);
    virtual nn::nex::qResult ReceiveNewArrivedObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                                      nn::nex::qVector<ListenerV1*> listeners,
                                                      nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV1>* pEventListener,
                                                      bool* pHasNext);
    virtual nn::nex::qResult ReceiveNewArrivedObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                                      nn::nex::qVector<ListenerV0*> listeners,
                                                      nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV0>* pEventListener,
                                                      bool* pHasNext);
    virtual bool GetRating(nn::nex::ProtocolCallContext* pContext, unsigned long long dataId, nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings,
                           unsigned long long accessPassword);
    virtual bool GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<unsigned long long>& dataIds,
                            nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults);
    virtual bool GetRatingWithLogDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating,
                                        nn::nex::DataStoreRatingLog* pRatingLog, u64 accessPassword);
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, ListenerV1* pListener);
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, unsigned char* pBuffer, unsigned int bufferSize,
                           unsigned int* pSize);

    template <typename Traits>
    bool GetObjectImpl(nn::nex::ProtocolCallContext* pContext, const typename Traits::TPrepareGetParam& param,
                       nn::nex::DataStoreGetObjectEventListenerTemplate<typename Traits::TDataId>* pListener, unsigned char* pBuffer, unsigned int bufferSize,
                       bool getsMeta, unsigned int* pSize);
    bool GetObjectImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, ListenerV1* pListener, unsigned char* pBuffer,
                       unsigned int bufferSize, unsigned int* pSize);
    template <typename Traits>
    bool PostObjectImpl(nn::nex::ProtocolCallContext* pContext, const typename Traits::TPreparePostParam& param, typename Traits::TPreparePostParam::TDataId* pDataId,
                        nn::nex::DataStorePostObjectEventListener* pListener, const unsigned char* pData, bool copiesData);
    bool UpdateObjectImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, nn::nex::DataStorePostObjectEventListener* pListener,
                          const unsigned char* pData, bool copiesData);
    bool CheckNotificationImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& url, nn::nex::String* pUrl, unsigned long long lastDataId,
                               unsigned long long* pLastDataId, unsigned int interval, unsigned int* pInterval, unsigned int principalId, bool* pIsLogicServerError,
                               bool getsUrl);
    template <typename Traits>
    nn::nex::qResult ReceiveNewArrivedObjectsImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                                  nn::nex::qVector<nn::nex::DataStoreGetObjectEventListenerTemplate<typename Traits::TDataId>*> listeners,
                                                  nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<Traits>* pEventListener, bool* pHasNext);
    bool GetRatingImpl(nn::nex::ProtocolCallContext* pContext, unsigned long long dataId, nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings,
                       unsigned long long accessPassword);
    bool GetRatingsImpl(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<unsigned long long>& dataIds,
                        nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults);

    u32 m_ThreadPriority;    // 0x60 (of the HTTP requests)
    bool m_UsesHttps;        // 0x64
    u32 m_MinBytesPerSecond; // 0x68 (0: no timeout of the requests)
    u32 m_MinTimeout;        // 0x6C (ms)
    u32 m_ChunkSize;         // 0x70 (of the posts)
    u32 m_HttpBufferSize;    // 0x74
};
} // namespace nex
} // namespace nn
