#pragma once

#include "decomp.h"
#include "nn/nex/nex_qVector.h"
#include <utility>

namespace nn {
namespace nex {
class ProtocolCallContext;
class qResult;
class DataStoreChangeMetaParam;
class DataStoreCompletePostParam;
class DataStoreCompletePostParamV1;
class DataStoreCompleteUpdateParam;
class DataStoreDeleteParam;
class DataStoreGetMetaParam;
class DataStoreGetNewArrivedNotificationsParam;
class DataStoreGetNotificationUrlParam;
class DataStoreGetSpecificMetaParam;
class DataStoreGetSpecificMetaParamV1;
class DataStoreMetaInfo;
class DataStoreNotification;
class DataStoreNotificationV1;
class DataStorePasswordInfo;
class DataStorePersistenceInfo;
class DataStorePrepareGetParam;
class DataStorePrepareGetParamV1;
class DataStorePreparePostParam;
class DataStorePreparePostParamV1;
class DataStorePrepareUpdateParam;
class DataStoreRateObjectParam;
class DataStoreRatingInfo;
class DataStoreRatingInfoWithSlot;
class DataStoreRatingLog;
class DataStoreRatingTarget;
class DataStoreReqGetAdditionalMeta;
class DataStoreReqGetInfo;
class DataStoreReqGetInfoV1;
class DataStoreReqGetNotificationUrlInfo;
class DataStoreReqPostInfo;
class DataStoreReqPostInfoV1;
class DataStoreReqUpdateInfo;
class DataStoreSearchParam;
class DataStoreSearchResult;
class DataStoreSpecificMetaInfo;
class DataStoreSpecificMetaInfoV1;
class DataStoreTouchObjectParam;

// The calls of the DataStore logic server that the DataStore jobs use (DataStoreLogicServerClient
// implements them). The names of the slots are ours, after the methods of the DataStore protocol
// they call; this build has no code of the interface itself.
// RTTI N2nn3nex35DataStoreLogicServerClientInterfaceE @ 0x008CF37C
class DataStoreLogicServerClientInterface
{
public:
    // (inline)
    DataStoreLogicServerClientInterface() {}
    virtual ~DataStoreLogicServerClientInterface() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    virtual bool PrepareGetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo) = 0; // slot 0x08
    virtual bool PrepareGetObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo) = 0; // slot 0x0C
    virtual bool PrepareGetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo,
                                              nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta) = 0; // slot 0x10
    // (the name is ours; DataStoreLogicServerClient has no call for it)
    virtual bool PrepareGetObjectOrMetaBinaryV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo,
                                                nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta) = 0; // slot 0x14
    virtual bool PreparePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, nn::nex::DataStoreReqPostInfo* pReqPostInfo) = 0; // slot 0x18
    virtual bool PreparePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, nn::nex::DataStoreReqPostInfoV1* pReqPostInfo) = 0; // slot 0x1C
    virtual bool CompletePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParam& param) = 0; // slot 0x20
    virtual bool CompletePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParamV1& param) = 0; // slot 0x24
    virtual bool CompletePostObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds) = 0; // slot 0x28
    virtual bool PrepareUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, nn::nex::DataStoreReqUpdateInfo* pReqUpdateInfo) = 0; // slot 0x2C
    virtual bool CompleteUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompleteUpdateParam& param) = 0; // slot 0x30
    virtual bool DeleteObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreDeleteParam& param) = 0; // slot 0x34
    virtual bool ChangeMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParam& param) = 0; // slot 0x38
    virtual bool GetMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetMetaParam& param, nn::nex::DataStoreMetaInfo* pMetaInfo) = 0; // slot 0x3C
    virtual bool GetMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStoreGetMetaParam& param,
                          nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x40
    virtual bool GetMetasMultipleParam(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>& params,
                                       nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x44
    virtual bool SearchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult) = 0; // slot 0x48
    virtual bool SearchObjectLight(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult) = 0; // slot 0x4C
    virtual bool GetNotificationUrl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNotificationUrlParam& param, nn::nex::DataStoreReqGetNotificationUrlInfo* pInfo) = 0; // slot 0x50
    virtual bool GetNewArrivedNotifications(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param,
                                            nn::nex::qVector<nn::nex::DataStoreNotification>* pResult, bool* pHasNext) = 0; // slot 0x54
    virtual bool GetNewArrivedNotificationsV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param,
                                              nn::nex::qVector<nn::nex::DataStoreNotificationV1>* pResult, bool* pHasNext) = 0; // slot 0x58
    virtual bool RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, const nn::nex::DataStoreRateObjectParam& param,
                            nn::nex::DataStoreRatingInfo* pRating) = 0; // slot 0x5C
    virtual bool GetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, u64 accessPassword) = 0; // slot 0x60
    virtual bool GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                            nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults,
                            u64 accessPassword) = 0; // slot 0x64
    virtual bool ResetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, u64 updatePassword) = 0; // slot 0x68
    virtual bool ResetRatings(nn::nex::ProtocolCallContext* pContext, const u64& dataId) = 0; // slot 0x6C
    virtual bool ResetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x70
    virtual bool GetSpecificMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParam& param,
                                 nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfo>* pMetaInfos) = 0; // slot 0x74
    virtual bool GetSpecificMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParamV1& param,
                                   nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfoV1>* pMetaInfos) = 0; // slot 0x78
    virtual bool PostMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId) = 0; // slot 0x7C
    virtual bool PostMetaBinaryWithDataId(nn::nex::ProtocolCallContext* pContext, u64 dataId, const nn::nex::DataStorePreparePostParam& param) = 0; // slot 0x80
    virtual bool PostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStorePreparePostParam& param,
                                            bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x84
    virtual bool PostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<std::pair<u64, nn::nex::DataStorePreparePostParam> >& params,
                                            bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x88
    virtual bool TouchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreTouchObjectParam& param) = 0; // slot 0x8C
    virtual bool GetRatingWithLog(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating,
                                  nn::nex::DataStoreRatingLog* pRatingLog, u64 accessPassword) = 0; // slot 0x90
    virtual bool GetPersistenceInfo(nn::nex::ProtocolCallContext* pContext, unsigned int ownerId, unsigned short persistenceSlotId,
                                    nn::nex::DataStorePersistenceInfo* pPersistenceInfo) = 0; // slot 0x94
    virtual bool GetPersistenceInfos(nn::nex::ProtocolCallContext* pContext, unsigned int ownerId, const nn::nex::qVector<u16>& persistenceSlotIds,
                                     nn::nex::qVector<nn::nex::DataStorePersistenceInfo>* pPersistenceInfo, nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0x98
    virtual bool PerpetuateObject(nn::nex::ProtocolCallContext* pContext, unsigned short persistenceSlotId, u64 dataId, bool deleteLastObject) = 0; // slot 0x9C
    virtual bool UnperpetuateObject(nn::nex::ProtocolCallContext* pContext, unsigned short persistenceSlotId, bool deleteLastObject) = 0; // slot 0xA0
    virtual bool GetPasswordInfo(nn::nex::ProtocolCallContext* pContext, u64 dataId, nn::nex::DataStorePasswordInfo* pPasswordInfo) = 0; // slot 0xA4
    virtual bool GetPasswordInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStorePasswordInfo>* pPasswordInfos,
                                  nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0xA8
    virtual bool GetObjectInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStoreReqGetInfo>* pInfos,
                                nn::nex::qVector<nn::nex::qResult>* pResults) = 0; // slot 0xAC
    // the principal of the default credentials (0 without)
    virtual u32 GetPrincipalId() = 0; // slot 0xB0
    // (the name is ours) the calls are made through the connection of the credentials
    virtual bool UsesConnection() = 0; // slot 0xB4
};
} // namespace nex
} // namespace nn
