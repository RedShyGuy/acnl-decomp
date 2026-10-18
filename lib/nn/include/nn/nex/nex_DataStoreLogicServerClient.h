#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreLogicServerClientInterface.h"
#include "nn/nex/nex_DataStoreProtocolClient.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_String.h"
#include "nn/cfg/CTR/CTR_Api.h"

namespace nn {
namespace nex {
// The client of the DataStore logic server: the calls of the DataStore protocol for the DataStore
// jobs, some of them with vectors made from single parameters. The names of the slots and members
// are ours (after the methods of the protocol), except those from the reference symbols.
// RTTI N2nn3nex26DataStoreLogicServerClientE @ 0x008CEF30
// vtable 0x008FEBAC (vptr 0x008FEBB4), offset_to_top 0, 67 entries
// vtable 0x008FECC0 (vptr 0x008FECC8), offset_to_top -16, 46 entries
class DataStoreLogicServerClient : public ::nn::nex::ServiceClient, public ::nn::nex::DataStoreLogicServerClientInterface
{
public:
    // the id of the DataStore protocol (and of the client)
    static const u16 PROTOCOL_ID = 115;

    // (inline in the constructor of DataStoreClientTemplate)
    DataStoreLogicServerClient() : ServiceClient(PROTOCOL_ID), m_Protocol(0)
    {
        m_Protocol.m_ProtocolId = PROTOCOL_ID;
        GetProtocolRegistry()->RegisterProtocol(PROTOCOL_ID, new DataStoreProtocolClient(0));
        m_IsProtocolRegistered = false;
        nn::cfg::CTR::Initialize();
    }
    virtual ~DataStoreLogicServerClient(); // 0x003B9798 slot 0x00
    // 0x003B9720 (deleting dtor)
    // 0x007754A4 (thunk)
    // 0x00775428 (deleting thunk)
    virtual bool Bind(nn::nex::Credentials* pCredentials); // 0x003B963C slot 0x0C | fefates:bytes
    virtual bool Unbind(); // 0x003B9670 slot 0x10 | fefates:bytes
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x003B9634 slot 0x20
    virtual bool PrepareGetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo); // 0x003A1AB4 slot 0x28
    // 0x00775008 (thunk)
    virtual bool PrepareGetObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo); // 0x003A33CC slot 0x2C
    // 0x00775040 (thunk)
    virtual bool PrepareGetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo, nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta); // 0x003B95E0 slot 0x30
    // 0x00775350 (thunk)
    // (not supported: false)
    virtual bool PrepareGetObjectOrMetaBinaryV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo,
                                                nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta); // 0x003B962C slot 0x34
    // 0x007753A0 (thunk)
    virtual bool PreparePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, nn::nex::DataStoreReqPostInfo* pReqPostInfo); // 0x003A2AC0 slot 0x38
    // 0x00775048 (thunk)
    virtual bool PreparePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, nn::nex::DataStoreReqPostInfoV1* pReqPostInfo); // 0x003A4A3C slot 0x3C
    // 0x00775080 (thunk)
    virtual bool CompletePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParam& param); // 0x003A2E38 slot 0x40
    // 0x00775090 (thunk)
    virtual bool CompletePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParamV1& param); // 0x003A5E90 slot 0x44
    // 0x00775098 (thunk)
    virtual bool CompletePostObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds); // 0x003A46E4 slot 0x48
    // 0x00775348 (thunk)
    virtual bool PostMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId); // 0x003A1080 slot 0x4C
    // 0x00774F70 (thunk)
    virtual bool PostMetaBinaryWithDataId(nn::nex::ProtocolCallContext* pContext, u64 dataId, const nn::nex::DataStorePreparePostParam& param); // 0x003B93F8 slot 0x50
    // 0x00775160 (thunk)
    virtual bool PostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStorePreparePostParam& param, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B9414 slot 0x54
    // 0x0077517C (thunk)
    virtual bool PostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<std::pair<u64, nn::nex::DataStorePreparePostParam> >& params, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B94BC slot 0x58
    // 0x00775224 (thunk)
    virtual bool PrepareUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, nn::nex::DataStoreReqUpdateInfo* pReqUpdateInfo); // 0x003A4BA4 slot 0x5C
    // 0x00775120 (thunk)
    virtual bool CompleteUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompleteUpdateParam& param); // 0x003A609C slot 0x60
    // 0x00775158 (thunk)
    virtual bool DeleteObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreDeleteParam& param); // 0x003A063C slot 0x64
    // 0x00774F48 (thunk)
    virtual bool DeleteObject(nn::nex::ProtocolCallContext* pContext, const u64& dataId); // 0x003B9024 slot 0x68
    virtual bool DeleteObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreDeleteParam>& params, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8EC0 slot 0x6C
    virtual bool DeleteObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8EE4 slot 0x70
    virtual bool ChangeMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParam& param); // 0x0039F948 slot 0x74
    // 0x00774E6C (thunk)
    virtual bool ChangeMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParamV1& param); // 0x003A04F0 slot 0x78
    virtual bool ChangeMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStoreChangeMetaParam& param, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8BE4 slot 0x7C
    virtual bool ChangeMetasV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStoreChangeMetaParamV1& param, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8C84 slot 0x80
    virtual bool ChangeMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>& params, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8B44 slot 0x84
    virtual bool ChangeMetasV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParamV1>& params, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8B94 slot 0x88
    virtual bool GetMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetMetaParam& param, nn::nex::DataStoreMetaInfo* pMetaInfo); // 0x0039F4AC slot 0x8C
    // 0x007753A8 (thunk)
    virtual bool GetMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStoreGetMetaParam& param, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B96C0 slot 0x90
    // 0x007753C8 (thunk)
    virtual bool GetMetasMultipleParam(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>& params, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B96A8 slot 0x94
    // 0x007753B0 (thunk)
    virtual bool SearchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult); // 0x003A093C slot 0x98
    // 0x00774F50 (thunk)
    virtual bool SearchObjectLight(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult); // 0x003A2C54 slot 0x9C
    // 0x00775088 (thunk)
    virtual bool GetNotificationUrl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNotificationUrlParam& param, nn::nex::DataStoreReqGetNotificationUrlInfo* pInfo); // 0x003A3044 slot 0xA0
    // 0x007750A0 (thunk)
    virtual bool GetNewArrivedNotifications(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param, nn::nex::qVector<nn::nex::DataStoreNotification>* pResult, bool* pHasNext); // 0x003B948C slot 0xA4
    // 0x007751F4 (thunk)
    virtual bool GetNewArrivedNotificationsV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param, nn::nex::qVector<nn::nex::DataStoreNotificationV1>* pResult, bool* pHasNext); // 0x003B94A4 slot 0xA8
    // 0x0077520C (thunk)
    virtual bool RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, const nn::nex::DataStoreRateObjectParam& param, nn::nex::DataStoreRatingInfo* pRating); // 0x003B8D24 slot 0xAC
    // 0x00774E74 (thunk)
    virtual bool RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets, const nn::nex::DataStoreRateObjectParam& param, bool transactional, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8D58 slot 0xB0
    virtual bool RateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets, const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& params, bool transactional, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8DF0 slot 0xB4
    virtual bool GetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, u64 accessPassword); // 0x003B96E4 slot 0xB8
    // 0x007753EC (thunk)
    virtual bool GetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults, u64 accessPassword); // 0x003B96FC slot 0xBC
    // 0x00775404 (thunk)
    virtual bool RateObjectsWithPosting(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets, const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& rateParams, const nn::nex::qVector<nn::nex::DataStorePreparePostParam>& postParams, bool transactional, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B92D0 slot 0xC0
    virtual bool RateObjectsWithPosting(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets, const nn::nex::DataStoreRateObjectParam& rateParam, const nn::nex::DataStorePreparePostParam& postParam, bool transactional, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B91F8 slot 0xC4
    virtual bool RateObjectWithPosting(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, const nn::nex::DataStoreRateObjectParam& rateParam, const nn::nex::DataStorePreparePostParam& postParam, nn::nex::DataStoreRatingInfo* pRating); // 0x003B9178 slot 0xC8
    virtual bool ResetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, u64 updatePassword); // 0x003A016C slot 0xCC
    // 0x00774EAC (thunk)
    virtual bool ResetRatings(nn::nex::ProtocolCallContext* pContext, const u64& dataId); // 0x003B8E5C slot 0xD0
    // 0x00774EDC (thunk)
    virtual bool ResetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, bool transactional, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B8E38 slot 0xD4
    // 0x00774EB8 (thunk)
    virtual bool GetSpecificMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParam& param, nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfo>* pMetaInfos); // 0x003A134C slot 0xD8
    // 0x00774FAC (thunk)
    virtual bool GetSpecificMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParamV1& param, nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfoV1>* pMetaInfos); // 0x003A2878 slot 0xDC
    // 0x00774FB4 (thunk)
    virtual bool TouchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreTouchObjectParam& param); // 0x003A02D0 slot 0xE0
    // 0x00774F40 (thunk)
    virtual bool GetRatingWithLog(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, nn::nex::DataStoreRatingLog* pRatingLog, u64 accessPassword); // 0x003B90C0 slot 0xE4
    // 0x00774FBC (thunk)
    virtual bool GetPersistenceInfo(nn::nex::ProtocolCallContext* pContext, unsigned int ownerId, unsigned short persistenceSlotId, nn::nex::DataStorePersistenceInfo* pPersistenceInfo); // 0x003B9134 slot 0xE8
    // 0x007750D0 (thunk)
    virtual bool GetPersistenceInfos(nn::nex::ProtocolCallContext* pContext, unsigned int ownerId, const nn::nex::qVector<u16>& persistenceSlotIds, nn::nex::qVector<nn::nex::DataStorePersistenceInfo>* pPersistenceInfo, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B910C slot 0xEC
    // 0x007750A8 (thunk)
    virtual bool PerpetuateObject(nn::nex::ProtocolCallContext* pContext, unsigned short persistenceSlotId, u64 dataId, bool deleteLastObject); // 0x003B90E4 slot 0xF0
    // 0x00774FE0 (thunk)
    virtual bool UnperpetuateObject(nn::nex::ProtocolCallContext* pContext, unsigned short persistenceSlotId, bool deleteLastObject); // 0x003B915C slot 0xF4
    // 0x007750F8 (thunk)
    virtual bool GetPasswordInfo(nn::nex::ProtocolCallContext* pContext, u64 dataId, nn::nex::DataStorePasswordInfo* pPasswordInfo); // 0x003B90A4 slot 0xF8
    // 0x00774F90 (thunk)
    virtual bool GetPasswordInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStorePasswordInfo>* pPasswordInfos, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B908C slot 0xFC
    // 0x00774F78 (thunk)
    virtual bool GetObjectInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStoreReqGetInfo>* pInfos, nn::nex::qVector<nn::nex::qResult>* pResults); // 0x003B9074 slot 0x100
    // 0x00774F58 (thunk)
    // the principal id of the default credentials (0: none)
    virtual u32 GetPrincipalId(); // 0x0072DDA0 slot 0x104
    virtual bool UsesConnection(); // 0x0072DDB0 slot 0x108

    // the extra data of a request: the platform, region and country of the console and the
    // location of the connection (inline in the original, with a copy out of line)
    void SetCTRExtraData(nn::nex::qVector<nn::nex::String>* pExtraData); // 0x0072D9B0

    bool m_IsProtocolRegistered;        // 0x14
    DataStoreProtocolClient m_Protocol; // 0x18
};
ASSERT_OFFSET(DataStoreLogicServerClient, m_Protocol, 0x18);
ASSERT_SIZE(DataStoreLogicServerClient, 0x5C);
} // namespace nex
} // namespace nn
