#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreApiVersionTraits.h"
#include "nn/nex/nex_DataStoreGetObjectEventListenerTemplate.h"
#include "nn/nex/nex_DataStoreNewArrivedObjects.h"
#include "nn/nex/nex_qMap.h"
#include "nn/nex/nex_qResult.h"
#include "nn/nex/nex_qVector.h"

namespace nn {
namespace nex {
class DataStorePostObjectEventListener;
class DataStorePrepareUpdateParam;
class DataStoreRatingInfo;
class DataStoreRatingInfoWithSlot;
class DataStoreRatingLog;
class DataStoreRatingTarget;
class ProtocolCallContext;
class String;

// The DataStore API of the application (DataStoreClientTemplate implements it): the calls with jobs
// (objects over HTTP, ratings, notifications) and some calls of the protocol ("Direct"). There is no
// code of the interface itself; the order of the slots is from the thunks of DataStoreClientTemplate
// (0x7790C8..0x779550), the names of the methods are ours.
// RTTI N2nn3nex24DataStoreClientInterfaceE @ 0x008CED54
class DataStoreClientInterface
{
public:
    typedef DataStoreGetObjectEventListenerTemplate<u32> ListenerV0;
    typedef DataStoreGetObjectEventListenerTemplate<u64> ListenerV1;

    // (inline)
    DataStoreClientInterface() {}
    virtual ~DataStoreClientInterface() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, ListenerV1* pListener) = 0; // slot 0x08
    // the object, or its meta binary (without data)
    virtual bool GetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, ListenerV1* pListener) = 0; // slot 0x0C
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, ListenerV0* pListener) = 0; // slot 0x10
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, unsigned char* pBuffer, unsigned int bufferSize,
                           unsigned int* pSize) = 0; // slot 0x14
    virtual bool PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, unsigned long long* pDataId,
                            nn::nex::DataStorePostObjectEventListener* pListener) = 0; // slot 0x18
    virtual bool PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, unsigned int* pDataId,
                            nn::nex::DataStorePostObjectEventListener* pListener) = 0; // slot 0x1C
    virtual bool PostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, unsigned long long* pDataId,
                            const unsigned char* pData, bool copiesData) = 0; // slot 0x20
    virtual bool PostMetaBinaryDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId) = 0; // slot 0x24
    virtual bool PostMetaBinaryWithDataIdDirect(nn::nex::ProtocolCallContext* pContext, u64 dataId, const nn::nex::DataStorePreparePostParam& param) = 0; // slot 0x28
    virtual bool PostMetaBinariesWithDataIdDirect(nn::nex::ProtocolCallContext* pContext,
                                                  const nn::nex::qVector<std::pair<u64, nn::nex::DataStorePreparePostParam> >& params, bool transactional,
                                                  nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x2C
    virtual bool UpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param,
                              nn::nex::DataStorePostObjectEventListener* pListener) = 0; // slot 0x30
    virtual bool UpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, const unsigned char* pData,
                              bool copiesData) = 0; // slot 0x34
    virtual bool CheckNotification(nn::nex::ProtocolCallContext* pContext, const nn::nex::String& url, nn::nex::String* pUrl, unsigned long long lastDataId,
                                   unsigned long long* pLastDataId, unsigned int interval, unsigned int* pInterval, unsigned int principalId,
                                   bool* pIsLogicServerError, bool getsUrl) = 0; // slot 0x38
    virtual nn::nex::qResult ReceiveNewArrivedObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                                      nn::nex::qVector<ListenerV1*> listeners,
                                                      nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV1>* pEventListener,
                                                      bool* pHasNext) = 0; // slot 0x3C
    virtual nn::nex::qResult ReceiveNewArrivedObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreNewArrivedObjectsState* pState,
                                                      nn::nex::qVector<ListenerV0*> listeners,
                                                      nn::nex::DataStoreNewArrivedObjectsEventListenerTemplate<nn::nex::DataStoreApiVersionTraitsV0>* pEventListener,
                                                      bool* pHasNext) = 0; // slot 0x40
    virtual bool GetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating,
                           u64 accessPassword) = 0; // slot 0x44
    virtual bool GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                            nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults,
                            u64 accessPassword) = 0; // slot 0x48
    virtual bool GetRating(nn::nex::ProtocolCallContext* pContext, unsigned long long dataId, nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo>* pRatings,
                           unsigned long long accessPassword) = 0; // slot 0x4C
    virtual bool GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<unsigned long long>& dataIds,
                            nn::nex::qVector<nn::nex::qMap<signed char, nn::nex::DataStoreRatingInfo> >* pRatings,
                            nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x50
    virtual bool GetRatingWithLogDirect(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating,
                                        nn::nex::DataStoreRatingLog* pRatingLog, u64 accessPassword) = 0; // slot 0x54
    // the object of a request of PrepareGetObject
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, ListenerV1* pListener) = 0; // slot 0x58
    virtual bool GetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreReqGetInfo& reqGetInfo, unsigned char* pBuffer, unsigned int bufferSize,
                           unsigned int* pSize) = 0; // slot 0x5C
};
} // namespace nex
} // namespace nn
