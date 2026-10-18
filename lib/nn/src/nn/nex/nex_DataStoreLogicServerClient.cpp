#include "nn/nex/nex_DataStoreLogicServerClient.h"
#include "nn/ac/CTR/detail/ac_Ac.h"
#include "nn/cfg/CTR/detail/detail_Api.h"
#include "nn/nex/nex_Credentials.h"
#include "nn/nex/nex_StringStream.h"
#include "nn/nex/nex_DataStoreChangeMetaParam.h"
#include "nn/nex/nex_DataStoreChangeMetaParamV1.h"
#include "nn/nex/nex_DataStoreCompletePostParam.h"
#include "nn/nex/nex_DataStoreCompletePostParamV1.h"
#include "nn/nex/nex_DataStoreCompleteUpdateParam.h"
#include "nn/nex/nex_DataStoreDeleteParam.h"
#include "nn/nex/nex_DataStoreGetMetaParam.h"
#include "nn/nex/nex_DataStoreGetNewArrivedNotificationsParam.h"
#include "nn/nex/nex_DataStoreGetNotificationUrlParam.h"
#include "nn/nex/nex_DataStoreGetSpecificMetaParam.h"
#include "nn/nex/nex_DataStoreGetSpecificMetaParamV1.h"
#include "nn/nex/nex_DataStoreMetaInfo.h"
#include "nn/nex/nex_DataStoreNotification.h"
#include "nn/nex/nex_DataStoreNotificationV1.h"
#include "nn/nex/nex_DataStorePasswordInfo.h"
#include "nn/nex/nex_DataStorePersistenceInfo.h"
#include "nn/nex/nex_DataStorePrepareGetParam.h"
#include "nn/nex/nex_DataStorePrepareGetParamV1.h"
#include "nn/nex/nex_DataStorePreparePostParam.h"
#include "nn/nex/nex_DataStorePreparePostParamV1.h"
#include "nn/nex/nex_DataStorePrepareUpdateParam.h"
#include "nn/nex/nex_DataStoreRateObjectParam.h"
#include "nn/nex/nex_DataStoreRatingInfo.h"
#include "nn/nex/nex_DataStoreRatingInfoWithSlot.h"
#include "nn/nex/nex_DataStoreRatingLog.h"
#include "nn/nex/nex_DataStoreRatingTarget.h"
#include "nn/nex/nex_DataStoreReqGetAdditionalMeta.h"
#include "nn/nex/nex_DataStoreReqGetInfo.h"
#include "nn/nex/nex_DataStoreReqGetInfoV1.h"
#include "nn/nex/nex_DataStoreReqGetNotificationUrlInfo.h"
#include "nn/nex/nex_DataStoreReqPostInfo.h"
#include "nn/nex/nex_DataStoreReqPostInfoV1.h"
#include "nn/nex/nex_DataStoreReqUpdateInfo.h"
#include "nn/nex/nex_DataStoreSearchParam.h"
#include "nn/nex/nex_DataStoreSearchResult.h"
#include "nn/nex/nex_DataStoreSpecificMetaInfo.h"
#include "nn/nex/nex_DataStoreSpecificMetaInfoV1.h"
#include "nn/nex/nex_DataStoreTouchObjectParam.h"

namespace nn {
namespace nex {
// 0x003A1AB4 slot 0x28
// 0x00775008 (thunk)
bool nn::nex::DataStoreLogicServerClient::PrepareGetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo)
{
    SetCTRExtraData(&const_cast<qVector<String>&>(param.m_ExtraData));
    return m_Protocol.CallPrepareGetObject(pContext, param, pReqGetInfo);
}

// 0x003A33CC slot 0x2C
// 0x00775040 (thunk)
bool nn::nex::DataStoreLogicServerClient::PrepareGetObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo)
{
    return m_Protocol.CallPrepareGetObjectV1(pContext, param, pReqGetInfo);
}

// 0x003B95E0 slot 0x30
// 0x00775350 (thunk)
bool nn::nex::DataStoreLogicServerClient::PrepareGetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo, nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta)
{
    SetCTRExtraData(&const_cast<qVector<String>&>(param.m_ExtraData));
    return m_Protocol.CallPrepareGetObjectOrMetaBinary(pContext, param, pReqGetInfo, pReqGetAdditionalMeta);
}

// 0x003B962C slot 0x34
// 0x007753A0 (thunk)
bool nn::nex::DataStoreLogicServerClient::PrepareGetObjectOrMetaBinaryV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo,
                                                                     nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta)
{
    return false;
}

// 0x003A2AC0 slot 0x38
// 0x00775048 (thunk)
bool nn::nex::DataStoreLogicServerClient::PreparePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, nn::nex::DataStoreReqPostInfo* pReqPostInfo)
{
    SetCTRExtraData(&const_cast<qVector<String>&>(param.m_ExtraData));
    return m_Protocol.CallPreparePostObject(pContext, param, pReqPostInfo);
}

// 0x003A4A3C slot 0x3C
// 0x00775080 (thunk)
bool nn::nex::DataStoreLogicServerClient::PreparePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, nn::nex::DataStoreReqPostInfoV1* pReqPostInfo)
{
    return m_Protocol.CallPreparePostObjectV1(pContext, param, pReqPostInfo);
}

// 0x003A2E38 slot 0x40
// 0x00775090 (thunk)
bool nn::nex::DataStoreLogicServerClient::CompletePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParam& param)
{
    return m_Protocol.CallCompletePostObject(pContext, param);
}

// 0x003A5E90 slot 0x44
// 0x00775098 (thunk)
bool nn::nex::DataStoreLogicServerClient::CompletePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParamV1& param)
{
    return m_Protocol.CallCompletePostObjectV1(pContext, param);
}

// 0x003A46E4 slot 0x48
// 0x00775348 (thunk)
bool nn::nex::DataStoreLogicServerClient::CompletePostObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds)
{
    return m_Protocol.CallCompletePostObjects(pContext, dataIds);
}

// 0x003A1080 slot 0x4C
// 0x00774F70 (thunk)
bool nn::nex::DataStoreLogicServerClient::PostMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId)
{
    return m_Protocol.CallPostMetaBinary(pContext, param, pDataId);
}

// 0x003B93F8 slot 0x50
// 0x00775160 (thunk)
bool nn::nex::DataStoreLogicServerClient::PostMetaBinaryWithDataId(nn::nex::ProtocolCallContext* pContext, u64 dataId, const nn::nex::DataStorePreparePostParam& param)
{
    return m_Protocol.CallPostMetaBinaryWithDataId(pContext, dataId, param);
}

// 0x003A4BA4 slot 0x5C
// 0x00775120 (thunk)
bool nn::nex::DataStoreLogicServerClient::PrepareUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, nn::nex::DataStoreReqUpdateInfo* pReqUpdateInfo)
{
    SetCTRExtraData(&const_cast<qVector<String>&>(param.m_ExtraData));
    return m_Protocol.CallPrepareUpdateObject(pContext, param, pReqUpdateInfo);
}

// 0x003A609C slot 0x60
// 0x00775158 (thunk)
bool nn::nex::DataStoreLogicServerClient::CompleteUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompleteUpdateParam& param)
{
    return m_Protocol.CallCompleteUpdateObject(pContext, param);
}

// 0x003A063C slot 0x64
// 0x00774F48 (thunk)
bool nn::nex::DataStoreLogicServerClient::DeleteObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreDeleteParam& param)
{
    return m_Protocol.CallDeleteObject(pContext, param);
}

// 0x003B8EC0 slot 0x6C
bool nn::nex::DataStoreLogicServerClient::DeleteObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreDeleteParam>& params, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallDeleteObjects(pContext, params, transactional, pResults);
}

// 0x0039F948 slot 0x74
// 0x00774E6C (thunk)
bool nn::nex::DataStoreLogicServerClient::ChangeMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParam& param)
{
    return m_Protocol.CallChangeMeta(pContext, param);
}

// 0x003A04F0 slot 0x78
bool nn::nex::DataStoreLogicServerClient::ChangeMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParamV1& param)
{
    return m_Protocol.CallChangeMetaV1(pContext, param);
}

// 0x0039F4AC slot 0x8C
// 0x007753A8 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetMetaParam& param, nn::nex::DataStoreMetaInfo* pMetaInfo)
{
    return m_Protocol.CallGetMeta(pContext, param, pMetaInfo);
}

// 0x003B96C0 slot 0x90
// 0x007753C8 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStoreGetMetaParam& param, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallGetMetas(pContext, dataIds, param, pMetaInfo, pResults);
}

// 0x003B96A8 slot 0x94
// 0x007753B0 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetMetasMultipleParam(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>& params, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallGetMetasMultipleParam(pContext, params, pMetaInfo, pResults);
}

// 0x003A093C slot 0x98
// 0x00774F50 (thunk)
bool nn::nex::DataStoreLogicServerClient::SearchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult)
{
    return m_Protocol.CallSearchObject(pContext, param, pSearchResult);
}

// 0x003A2C54 slot 0x9C
// 0x00775088 (thunk)
bool nn::nex::DataStoreLogicServerClient::SearchObjectLight(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult)
{
    return m_Protocol.CallSearchObjectLight(pContext, param, pSearchResult);
}

// 0x003A3044 slot 0xA0
// 0x007750A0 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetNotificationUrl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNotificationUrlParam& param, nn::nex::DataStoreReqGetNotificationUrlInfo* pInfo)
{
    return m_Protocol.CallGetNotificationUrl(pContext, param, pInfo);
}

// 0x003B948C slot 0xA4
// 0x007751F4 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetNewArrivedNotifications(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param, nn::nex::qVector<nn::nex::DataStoreNotification>* pResult, bool* pHasNext)
{
    return m_Protocol.CallGetNewArrivedNotifications(pContext, param, pResult, pHasNext);
}

// 0x003B94A4 slot 0xA8
// 0x0077520C (thunk)
bool nn::nex::DataStoreLogicServerClient::GetNewArrivedNotificationsV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param, nn::nex::qVector<nn::nex::DataStoreNotificationV1>* pResult, bool* pHasNext)
{
    return m_Protocol.CallGetNewArrivedNotificationsV1(pContext, param, pResult, pHasNext);
}

// 0x003B96E4 slot 0xB8
// 0x007753EC (thunk)
bool nn::nex::DataStoreLogicServerClient::GetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, u64 accessPassword)
{
    return m_Protocol.CallGetRating(pContext, target, pRating, accessPassword);
}

// 0x003B96FC slot 0xBC
// 0x00775404 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults, u64 accessPassword)
{
    return m_Protocol.CallGetRatings(pContext, dataIds, pRatings, pResults, accessPassword);
}

// 0x003A016C slot 0xCC
// 0x00774EAC (thunk)
bool nn::nex::DataStoreLogicServerClient::ResetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, u64 updatePassword)
{
    return m_Protocol.CallResetRating(pContext, target, updatePassword);
}

// 0x003B8E38 slot 0xD4
// 0x00774EB8 (thunk)
bool nn::nex::DataStoreLogicServerClient::ResetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallResetRatings(pContext, dataIds, transactional, pResults);
}

// 0x003A134C slot 0xD8
// 0x00774FAC (thunk)
bool nn::nex::DataStoreLogicServerClient::GetSpecificMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParam& param, nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfo>* pMetaInfos)
{
    return m_Protocol.CallGetSpecificMeta(pContext, param, pMetaInfos);
}

// 0x003A2878 slot 0xDC
// 0x00774FB4 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetSpecificMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParamV1& param, nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfoV1>* pMetaInfos)
{
    return m_Protocol.CallGetSpecificMetaV1(pContext, param, pMetaInfos);
}

// 0x003A02D0 slot 0xE0
// 0x00774F40 (thunk)
bool nn::nex::DataStoreLogicServerClient::TouchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreTouchObjectParam& param)
{
    return m_Protocol.CallTouchObject(pContext, param);
}

// 0x003B90C0 slot 0xE4
// 0x00774FBC (thunk)
bool nn::nex::DataStoreLogicServerClient::GetRatingWithLog(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, nn::nex::DataStoreRatingLog* pRatingLog, u64 accessPassword)
{
    return m_Protocol.CallGetRatingWithLog(pContext, target, pRating, pRatingLog, accessPassword);
}

// 0x003B9134 slot 0xE8
// 0x007750D0 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetPersistenceInfo(nn::nex::ProtocolCallContext* pContext, unsigned int ownerId, unsigned short persistenceSlotId, nn::nex::DataStorePersistenceInfo* pPersistenceInfo)
{
    return m_Protocol.CallGetPersistenceInfo(pContext, ownerId, persistenceSlotId, pPersistenceInfo);
}

// 0x003B910C slot 0xEC
// 0x007750A8 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetPersistenceInfos(nn::nex::ProtocolCallContext* pContext, unsigned int ownerId, const nn::nex::qVector<u16>& persistenceSlotIds, nn::nex::qVector<nn::nex::DataStorePersistenceInfo>* pPersistenceInfo, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallGetPersistenceInfos(pContext, ownerId, persistenceSlotIds, pPersistenceInfo, pResults);
}

// 0x003B90E4 slot 0xF0
// 0x00774FE0 (thunk)
bool nn::nex::DataStoreLogicServerClient::PerpetuateObject(nn::nex::ProtocolCallContext* pContext, unsigned short persistenceSlotId, u64 dataId, bool deleteLastObject)
{
    return m_Protocol.CallPerpetuateObject(pContext, persistenceSlotId, dataId, deleteLastObject);
}

// 0x003B915C slot 0xF4
// 0x007750F8 (thunk)
bool nn::nex::DataStoreLogicServerClient::UnperpetuateObject(nn::nex::ProtocolCallContext* pContext, unsigned short persistenceSlotId, bool deleteLastObject)
{
    return m_Protocol.CallUnperpetuateObject(pContext, persistenceSlotId, deleteLastObject);
}

// 0x003B90A4 slot 0xF8
// 0x00774F90 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetPasswordInfo(nn::nex::ProtocolCallContext* pContext, u64 dataId, nn::nex::DataStorePasswordInfo* pPasswordInfo)
{
    return m_Protocol.CallGetPasswordInfo(pContext, dataId, pPasswordInfo);
}

// 0x003B908C slot 0xFC
// 0x00774F78 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetPasswordInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStorePasswordInfo>* pPasswordInfos, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallGetPasswordInfos(pContext, dataIds, pPasswordInfos, pResults);
}

// 0x003B9074 slot 0x100
// 0x00774F58 (thunk)
bool nn::nex::DataStoreLogicServerClient::GetObjectInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStoreReqGetInfo>* pInfos, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    return m_Protocol.CallGetObjectInfos(pContext, dataIds, pInfos, pResults);
}

// 0x003B9414 slot 0x54
// 0x0077517C (thunk)
bool nn::nex::DataStoreLogicServerClient::PostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                                                                     const nn::nex::DataStorePreparePostParam& param, bool transactional,
                                                                     nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStorePreparePostParam> params;
    params.push_back(param);
    return m_Protocol.CallPostMetaBinariesWithDataId(pContext, dataIds, params, transactional, pResults);
}

// 0x003B94BC slot 0x58
// 0x00775224 (thunk)
bool nn::nex::DataStoreLogicServerClient::PostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext,
                                                                     const nn::nex::qVector<std::pair<u64, nn::nex::DataStorePreparePostParam> >& params,
                                                                     bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<u64> dataIds;
    qVector<DataStorePreparePostParam> postParams;
    for (const std::pair<u64, DataStorePreparePostParam>* p = params.begin(); p != params.end(); p++) {
        dataIds.push_back(p->first);
        postParams.push_back(p->second);
    }
    return m_Protocol.CallPostMetaBinariesWithDataId(pContext, dataIds, postParams, transactional, pResults);
}

// 0x003B9024 slot 0x68
bool nn::nex::DataStoreLogicServerClient::DeleteObject(nn::nex::ProtocolCallContext* pContext, const u64& dataId)
{
    DataStoreDeleteParam param;
    param.m_DataId = dataId;
    return m_Protocol.CallDeleteObject(pContext, param);
}

// 0x003B8EE4 slot 0x70
bool nn::nex::DataStoreLogicServerClient::DeleteObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, bool transactional,
                                                        nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStoreDeleteParam> params;
    DataStoreDeleteParam param;
    for (const u64* p = dataIds.begin(); p != dataIds.end(); p++) {
        param.m_DataId = *p;
        params.push_back(param);
    }
    return m_Protocol.CallDeleteObjects(pContext, params, transactional, pResults);
}

// 0x003B8BE4 slot 0x7C
bool nn::nex::DataStoreLogicServerClient::ChangeMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                                                      const nn::nex::DataStoreChangeMetaParam& param, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStoreChangeMetaParam> params;
    params.push_back(param);
    return m_Protocol.CallChangeMetas(pContext, dataIds, params, transactional, pResults);
}

// 0x003B8C84 slot 0x80
bool nn::nex::DataStoreLogicServerClient::ChangeMetasV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds,
                                                        const nn::nex::DataStoreChangeMetaParamV1& param, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStoreChangeMetaParamV1> params;
    params.push_back(param);
    return m_Protocol.CallChangeMetasV1(pContext, dataIds, params, transactional, pResults);
}

// 0x003B8B44 slot 0x84
bool nn::nex::DataStoreLogicServerClient::ChangeMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>& params,
                                                      bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    // (the data ids are in the parameters)
    qVector<u64> dataIds;
    return m_Protocol.CallChangeMetas(pContext, dataIds, params, transactional, pResults);
}

// 0x003B8B94 slot 0x88
bool nn::nex::DataStoreLogicServerClient::ChangeMetasV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParamV1>& params,
                                                        bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    // (the data ids are in the parameters)
    qVector<u64> dataIds;
    return m_Protocol.CallChangeMetasV1(pContext, dataIds, params, transactional, pResults);
}

// 0x003B8D24 slot 0xAC | fefates:bytes
// 0x00774E74 (thunk)
bool nn::nex::DataStoreLogicServerClient::RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target,
                                                     const nn::nex::DataStoreRateObjectParam& param, nn::nex::DataStoreRatingInfo* pRating)
{
    bool fetchRatings = pRating != nullptr;
    return m_Protocol.CallRateObject(pContext, target, param, fetchRatings, pRating);
}

// 0x003B8D58 slot 0xB0
bool nn::nex::DataStoreLogicServerClient::RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets,
                                                     const nn::nex::DataStoreRateObjectParam& param, bool transactional,
                                                     nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStoreRateObjectParam> params;
    params.push_back(param);
    bool fetchRatings = pRatings != nullptr;
    return m_Protocol.CallRateObjects(pContext, targets, params, transactional, fetchRatings, pRatings, pResults);
}

// 0x003B8DF0 slot 0xB4 | fefates:bytes
bool nn::nex::DataStoreLogicServerClient::RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets,
                                                     const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& params, bool transactional,
                                                     nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    bool fetchRatings = pRatings != nullptr;
    return m_Protocol.CallRateObjects(pContext, targets, params, transactional, fetchRatings, pRatings, pResults);
}

// 0x003B92D0 slot 0xC0
bool nn::nex::DataStoreLogicServerClient::RateObjectsWithPosting(nn::nex::ProtocolCallContext* pContext,
                                                                 const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets,
                                                                 const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& rateParams,
                                                                 const nn::nex::qVector<nn::nex::DataStorePreparePostParam>& postParams, bool transactional,
                                                                 nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    // (the original also checks that the post parameters have the rating slots of the targets; the
    // result of the check was only logged in this build)
    bool fetchRatings = pRatings != nullptr;
    return m_Protocol.CallRateObjectsWithPosting(pContext, targets, rateParams, postParams, transactional, fetchRatings, pRatings, pResults);
}

// 0x003B91F8 slot 0xC4
bool nn::nex::DataStoreLogicServerClient::RateObjectsWithPosting(nn::nex::ProtocolCallContext* pContext,
                                                                 const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets,
                                                                 const nn::nex::DataStoreRateObjectParam& rateParam, const nn::nex::DataStorePreparePostParam& postParam,
                                                                 bool transactional, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings,
                                                                 nn::nex::qVector<nn::nex::qResult>* pResults)
{
    qVector<DataStoreRateObjectParam> rateParams;
    rateParams.push_back(rateParam);
    qVector<DataStorePreparePostParam> postParams;
    postParams.push_back(postParam);
    return RateObjectsWithPosting(pContext, targets, rateParams, postParams, transactional, pRatings, pResults);
}

// 0x003B9178 slot 0xC8
bool nn::nex::DataStoreLogicServerClient::RateObjectWithPosting(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target,
                                                                const nn::nex::DataStoreRateObjectParam& rateParam, const nn::nex::DataStorePreparePostParam& postParam,
                                                                nn::nex::DataStoreRatingInfo* pRating)
{
    // the slot of the target must be one of the rating slots of the post
    bool hasSlot = false;
    for (const DataStoreRatingInitParamWithSlot* p = postParam.m_RatingInitParams.begin(); p != postParam.m_RatingInitParams.end(); p++) {
        if (p->m_Slot == target.m_Slot) {
            hasSlot = true;
        }
    }
    if (!hasSlot) {
        return false;
    }
    bool fetchRatings = pRating != nullptr;
    return m_Protocol.CallRateObjectWithPosting(pContext, target, rateParam, postParam, fetchRatings, pRating);
}

// 0x003B8E5C slot 0xD0
// 0x00774EDC (thunk)
bool nn::nex::DataStoreLogicServerClient::ResetRatings(nn::nex::ProtocolCallContext* pContext, const u64& dataId)
{
    qVector<u64> dataIds;
    dataIds.push_back(dataId);
    bool transactional = true;
    return m_Protocol.CallResetRatings(pContext, dataIds, transactional, nullptr);
}

// 0x003B9634 slot 0x20
void nn::nex::DataStoreLogicServerClient::UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials)
{
    m_Protocol.SetDefaultCredentials(pCredentials);
}

// 0x003B963C slot 0x0C | fefates:bytes
bool nn::nex::DataStoreLogicServerClient::Bind(nn::nex::Credentials* pCredentials)
{
    if (!ServiceClient::Bind(pCredentials)) {
        return false;
    }
    if (!RegisterProtocol(&m_Protocol)) {
        return false;
    }
    m_IsProtocolRegistered = true;
    return true;
}

// 0x003B9670 slot 0x10 | fefates:bytes
bool nn::nex::DataStoreLogicServerClient::Unbind()
{
    if (!ServiceClient::Unbind()) {
        return false;
    }
    if (!UnregisterProtocol(&m_Protocol)) {
        return false;
    }
    m_IsProtocolRegistered = false;
    return true;
}

// 0x003B9798 slot 0x00
// 0x003B9720 (deleting dtor)
// 0x007754A4 (thunk)
// 0x00775428 (deleting thunk)
nn::nex::DataStoreLogicServerClient::~DataStoreLogicServerClient()
{
    if (m_IsProtocolRegistered) {
        DataStoreLogicServerClient::Unbind();
    }
    nn::cfg::CTR::Finalize();
}

// 0x0072DDA0 slot 0x104
// 0x00775514 (thunk)
u32 nn::nex::DataStoreLogicServerClient::GetPrincipalId()
{
    return m_pDefaultCredentials != nullptr ? m_pDefaultCredentials->m_PrincipalId : 0;
}

// 0x0072DDB0 slot 0x108
// 0x00775524 (thunk)
bool nn::nex::DataStoreLogicServerClient::UsesConnection()
{
    return true;
}

// 0x0072D9B0
void nn::nex::DataStoreLogicServerClient::SetCTRExtraData(nn::nex::qVector<nn::nex::String>* pExtraData)
{
    const u32 EXTRA_DATA_COUNT = 6;
    pExtraData->ResizeInline(EXTRA_DATA_COUNT, String(L""));
    (*pExtraData)[0] = L"CTR";
    StringStream stream;
    nn::cfg::CTR::CfgRegionCode region = nn::cfg::CTR::GetRegion();
    stream << static_cast<int>(region);
    (*pExtraData)[1] = stream.m_pBuffer;
    const char* pRegion = nn::cfg::CTR::GetRegionCodeA3(region);
    if (pRegion != nullptr) {
        (*pExtraData)[2] = pRegion;
    }
    stream.Clear();
    nn::cfg::CTR::CfgCountryCode country = nn::cfg::CTR::GetCountry();
    stream << static_cast<int>(country);
    (*pExtraData)[3] = stream.m_pBuffer;
    char countryCode[3];
    if (nn::cfg::CTR::detail::GetCountryCodeString(countryCode, country).IsSuccess()) {
        (*pExtraData)[4] = countryCode;
    }
    u8 location[12];
    if (nn::ac::CTR::detail::Ac::GetConnectingLocationEntry(location).IsSuccess()) {
        location[11] = 0;
        (*pExtraData)[5] = reinterpret_cast<const char*>(location);
    }
}

} // namespace nex
} // namespace nn
