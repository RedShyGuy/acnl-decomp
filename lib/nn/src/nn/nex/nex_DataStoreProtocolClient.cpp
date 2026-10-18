#include "nn/nex/nex_DataStoreProtocolClient.h"
#include "nn/nex/nex_ClientProtocolCall.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_ProtocolRegistry.h"
#include "nn/nex/nex_RVClientCore.h"
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
// 0x00ADD940 (name is ours)
DataStoreProtocolClient::ProtoReturnFunction DataStoreProtocolClient::s_ProtoReturnFunctions[METHOD_COUNT];

namespace {
// the method is not known
const int RESULT_UNKNOWN_METHOD = static_cast<int>(0x80010002);

// a bool result (the name is ours)
struct BoolType
{
    typedef bool Type;
    static void Extract(Message* pMessage, bool* pValue) { *pMessage >> *pValue; }
};

// (inline) a result of the call: into its return value, or into a temporary if there is none
template <typename E>
inline void ExtractReturn(Message* pMessage, ProtocolCallContext* pContext, u32 index)
{
    typename E::Type* pValue = static_cast<typename E::Type*>(pContext->GetReturnValuePtr(index));
    if (pValue != nullptr) {
        E::Extract(pMessage, pValue);
    } else {
        typename E::Type temporary[1];
        E::Extract(pMessage, temporary);
    }
}
} // namespace

using namespace ClientProtocolCall;

// 0x0039F4B4
bool nn::nex::DataStoreProtocolClient::CallGetMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetMetaParam& param, nn::nex::DataStoreMetaInfo* pMetaInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_META)) {
        return false;
    }
    DDLClassMetaType<DataStoreGetMetaParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pMetaInfo);
    }
    s_ProtoReturnFunctions[METHOD_GET_META - 1] = ProtoReturn_GetMeta;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039F614
bool nn::nex::DataStoreProtocolClient::CallGetMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::DataStoreGetMetaParam& param, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_METAS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    DDLClassMetaType<DataStoreGetMetaParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pMetaInfo);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_GET_METAS - 1] = ProtoReturn_GetMetas;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039F7D4
bool nn::nex::DataStoreProtocolClient::CallGetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, const u64& accessPassword)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_RATING)) {
        return false;
    }
    DDLClassMetaType<DataStoreRatingTarget>::Add(&message, target);
    message.AppendRaw(reinterpret_cast<const u8*>(&accessPassword), sizeof(accessPassword));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRating);
    }
    s_ProtoReturnFunctions[METHOD_GET_RATING - 1] = ProtoReturn_GetRating;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039F950
bool nn::nex::DataStoreProtocolClient::CallChangeMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CHANGE_META)) {
        return false;
    }
    DDLClassMetaType<DataStoreChangeMetaParam>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_CHANGE_META - 1] = ProtoReturn_ChangeMeta;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039FA94
bool nn::nex::DataStoreProtocolClient::CallGetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::qVector<nn::nex::DataStoreRatingInfoWithSlot> >* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults, const u64& accessPassword)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_RATINGS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    message.AppendRaw(reinterpret_cast<const u8*>(&accessPassword), sizeof(accessPassword));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRatings);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_GET_RATINGS - 1] = ProtoReturn_GetRatings;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039FC54
bool nn::nex::DataStoreProtocolClient::CallRateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, const nn::nex::DataStoreRateObjectParam& param, const bool& fetchRatings, nn::nex::DataStoreRatingInfo* pRating)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_RATE_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStoreRatingTarget>::Add(&message, target);
    DDLClassMetaType<DataStoreRateObjectParam>::Add(&message, param);
    message << fetchRatings;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRating);
    }
    s_ProtoReturnFunctions[METHOD_RATE_OBJECT - 1] = ProtoReturn_RateObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039FDD0
bool nn::nex::DataStoreProtocolClient::CallChangeMetas(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParam>& params, const bool& transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CHANGE_METAS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    _Type_qvector<DDLClassMetaType<DataStoreChangeMetaParam> >::Add(&message, params);
    message << transactional;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_CHANGE_METAS - 1] = ProtoReturn_ChangeMetas;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x0039FFDC
bool nn::nex::DataStoreProtocolClient::CallRateObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets, const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& params, const bool& transactional, const bool& fetchRatings, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_RATE_OBJECTS)) {
        return false;
    }
    _Type_qvector<DDLClassMetaType<DataStoreRatingTarget> >::Add(&message, targets);
    _Type_qvector<DDLClassMetaType<DataStoreRateObjectParam> >::Add(&message, params);
    message << transactional;
    message << fetchRatings;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRatings);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_RATE_OBJECTS - 1] = ProtoReturn_RateObjects;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0178
bool nn::nex::DataStoreProtocolClient::CallResetRating(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, const u64& updatePassword)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_RESET_RATING)) {
        return false;
    }
    DDLClassMetaType<DataStoreRatingTarget>::Add(&message, target);
    message.AppendRaw(reinterpret_cast<const u8*>(&updatePassword), sizeof(updatePassword));
    s_ProtoReturnFunctions[METHOD_RESET_RATING - 1] = ProtoReturn_ResetRating;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A02D8
bool nn::nex::DataStoreProtocolClient::CallTouchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreTouchObjectParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_TOUCH_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStoreTouchObjectParam>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_TOUCH_OBJECT - 1] = ProtoReturn_TouchObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A04F8
bool nn::nex::DataStoreProtocolClient::CallChangeMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreChangeMetaParamV1& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CHANGE_META_V1)) {
        return false;
    }
    DDLClassMetaType<DataStoreChangeMetaParamV1>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_CHANGE_META_V1 - 1] = ProtoReturn_ChangeMetaV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0644
bool nn::nex::DataStoreProtocolClient::CallDeleteObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreDeleteParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_DELETE_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStoreDeleteParam>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_DELETE_OBJECT - 1] = ProtoReturn_DeleteObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0788
bool nn::nex::DataStoreProtocolClient::CallResetRatings(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const bool& transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_RESET_RATINGS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    message << transactional;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_RESET_RATINGS - 1] = ProtoReturn_ResetRatings;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0944
bool nn::nex::DataStoreProtocolClient::CallSearchObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_SEARCH_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStoreSearchParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pSearchResult);
    }
    s_ProtoReturnFunctions[METHOD_SEARCH_OBJECT - 1] = ProtoReturn_SearchObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0AA4
bool nn::nex::DataStoreProtocolClient::CallChangeMetasV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::qVector<nn::nex::DataStoreChangeMetaParamV1>& params, const bool& transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_CHANGE_METAS_V1)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    _Type_qvector<DDLClassMetaType<DataStoreChangeMetaParamV1> >::Add(&message, params);
    message << transactional;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_CHANGE_METAS_V1 - 1] = ProtoReturn_ChangeMetasV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0C60
bool nn::nex::DataStoreProtocolClient::CallDeleteObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreDeleteParam>& params, const bool& transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_DELETE_OBJECTS)) {
        return false;
    }
    _Type_qvector<DDLClassMetaType<DataStoreDeleteParam> >::Add(&message, params);
    message << transactional;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_DELETE_OBJECTS - 1] = ProtoReturn_DeleteObjects;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A0ECC
bool nn::nex::DataStoreProtocolClient::CallGetObjectInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStoreReqGetInfo>* pInfos, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_OBJECT_INFOS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pInfos);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_GET_OBJECT_INFOS - 1] = ProtoReturn_GetObjectInfos;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A1088
bool nn::nex::DataStoreProtocolClient::CallPostMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, u64* pDataId)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_POST_META_BINARY)) {
        return false;
    }
    DDLClassMetaType<DataStorePreparePostParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pDataId);
    }
    s_ProtoReturnFunctions[METHOD_POST_META_BINARY - 1] = ProtoReturn_PostMetaBinary;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A11E8
bool nn::nex::DataStoreProtocolClient::CallGetPasswordInfo(nn::nex::ProtocolCallContext* pContext, const u64& dataId, nn::nex::DataStorePasswordInfo* pPasswordInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_PASSWORD_INFO)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&dataId), sizeof(dataId));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pPasswordInfo);
    }
    s_ProtoReturnFunctions[METHOD_GET_PASSWORD_INFO - 1] = ProtoReturn_GetPasswordInfo;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A1354
bool nn::nex::DataStoreProtocolClient::CallGetSpecificMeta(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParam& param, nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfo>* pMetaInfos)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_SPECIFIC_META)) {
        return false;
    }
    DDLClassMetaType<DataStoreGetSpecificMetaParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pMetaInfos);
    }
    s_ProtoReturnFunctions[METHOD_GET_SPECIFIC_META - 1] = ProtoReturn_GetSpecificMeta;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A1594
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetMeta(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreMetaInfo>>(pMessage, pContext, 0);
}

// 0x003A1610
bool nn::nex::DataStoreProtocolClient::CallGetPasswordInfos(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, nn::nex::qVector<nn::nex::DataStorePasswordInfo>* pPasswordInfos, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_PASSWORD_INFOS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pPasswordInfos);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_GET_PASSWORD_INFOS - 1] = ProtoReturn_GetPasswordInfos;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A17C4
bool nn::nex::DataStoreProtocolClient::CallGetRatingWithLog(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, nn::nex::DataStoreRatingInfo* pRating, nn::nex::DataStoreRatingLog* pRatingLog, const u64& accessPassword)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_RATING_WITH_LOG)) {
        return false;
    }
    DDLClassMetaType<DataStoreRatingTarget>::Add(&message, target);
    message.AppendRaw(reinterpret_cast<const u8*>(&accessPassword), sizeof(accessPassword));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRating);
        pContext->AddReturnValue(pRatingLog);
    }
    s_ProtoReturnFunctions[METHOD_GET_RATING_WITH_LOG - 1] = ProtoReturn_GetRatingWithLog;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A1948
bool nn::nex::DataStoreProtocolClient::CallPerpetuateObject(nn::nex::ProtocolCallContext* pContext, const u16& persistenceSlotId, const u64& dataId, const bool& deleteLastObject)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PERPETUATE_OBJECT)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&persistenceSlotId), sizeof(persistenceSlotId));
    message.AppendRaw(reinterpret_cast<const u8*>(&dataId), sizeof(dataId));
    message << deleteLastObject;
    s_ProtoReturnFunctions[METHOD_PERPETUATE_OBJECT - 1] = ProtoReturn_PerpetuateObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A1AE8
bool nn::nex::DataStoreProtocolClient::CallPrepareGetObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PREPARE_GET_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStorePrepareGetParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pReqGetInfo);
    }
    s_ProtoReturnFunctions[METHOD_PREPARE_GET_OBJECT - 1] = ProtoReturn_PrepareGetObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A1C48
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetMetas(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreMetaInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003A2880
bool nn::nex::DataStoreProtocolClient::CallGetSpecificMetaV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetSpecificMetaParamV1& param, nn::nex::qVector<nn::nex::DataStoreSpecificMetaInfoV1>* pMetaInfos)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_SPECIFIC_META_V1)) {
        return false;
    }
    DDLClassMetaType<DataStoreGetSpecificMetaParamV1>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pMetaInfos);
    }
    s_ProtoReturnFunctions[METHOD_GET_SPECIFIC_META_V1 - 1] = ProtoReturn_GetSpecificMetaV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A2AF4
bool nn::nex::DataStoreProtocolClient::CallPreparePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParam& param, nn::nex::DataStoreReqPostInfo* pReqPostInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PREPARE_POST_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStorePreparePostParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pReqPostInfo);
    }
    s_ProtoReturnFunctions[METHOD_PREPARE_POST_OBJECT - 1] = ProtoReturn_PreparePostObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A2C5C
bool nn::nex::DataStoreProtocolClient::CallSearchObjectLight(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreSearchParam& param, nn::nex::DataStoreSearchResult* pSearchResult)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_SEARCH_OBJECT_LIGHT)) {
        return false;
    }
    DDLClassMetaType<DataStoreSearchParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pSearchResult);
    }
    s_ProtoReturnFunctions[METHOD_SEARCH_OBJECT_LIGHT - 1] = ProtoReturn_SearchObjectLight;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A2DBC
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetRating(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreRatingInfo>>(pMessage, pContext, 0);
}

// 0x003A2E40
bool nn::nex::DataStoreProtocolClient::CallCompletePostObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_COMPLETE_POST_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStoreCompletePostParam>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_COMPLETE_POST_OBJECT - 1] = ProtoReturn_CompletePostObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A304C
bool nn::nex::DataStoreProtocolClient::CallGetNotificationUrl(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNotificationUrlParam& param, nn::nex::DataStoreReqGetNotificationUrlInfo* pInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_NOTIFICATION_URL)) {
        return false;
    }
    DDLClassMetaType<DataStoreGetNotificationUrlParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pInfo);
    }
    s_ProtoReturnFunctions[METHOD_GET_NOTIFICATION_URL - 1] = ProtoReturn_GetNotificationUrl;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A3254
bool nn::nex::DataStoreProtocolClient::CallGetPersistenceInfo(nn::nex::ProtocolCallContext* pContext, const u32& ownerId, const u16& persistenceSlotId, nn::nex::DataStorePersistenceInfo* pPersistenceInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_PERSISTENCE_INFO)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&ownerId), sizeof(ownerId));
    message.AppendRaw(reinterpret_cast<const u8*>(&persistenceSlotId), sizeof(persistenceSlotId));
    if (pContext != nullptr) {
        pContext->AddReturnValue(pPersistenceInfo);
    }
    s_ProtoReturnFunctions[METHOD_GET_PERSISTENCE_INFO - 1] = ProtoReturn_GetPersistenceInfo;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A33D4
bool nn::nex::DataStoreProtocolClient::CallPrepareGetObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParamV1& param, nn::nex::DataStoreReqGetInfoV1* pReqGetInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PREPARE_GET_OBJECT_V1)) {
        return false;
    }
    DDLClassMetaType<DataStorePrepareGetParamV1>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pReqGetInfo);
    }
    s_ProtoReturnFunctions[METHOD_PREPARE_GET_OBJECT_V1 - 1] = ProtoReturn_PrepareGetObjectV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A35F0
bool nn::nex::DataStoreProtocolClient::CallUnperpetuateObject(nn::nex::ProtocolCallContext* pContext, const u16& persistenceSlotId, const bool& deleteLastObject)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_UNPERPETUATE_OBJECT)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&persistenceSlotId), sizeof(persistenceSlotId));
    message << deleteLastObject;
    s_ProtoReturnFunctions[METHOD_UNPERPETUATE_OBJECT - 1] = ProtoReturn_UnperpetuateObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A3748
void nn::nex::DataStoreProtocolClient::ProtoReturn_ChangeMeta(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003A374C
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetRatings(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<_Type_qvector<DDLClassMetaType<DataStoreRatingInfoWithSlot> > >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003A4668
void nn::nex::DataStoreProtocolClient::ProtoReturn_RateObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreRatingInfo>>(pMessage, pContext, 0);
}

// 0x003A46EC
bool nn::nex::DataStoreProtocolClient::CallCompletePostObjects(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_COMPLETE_POST_OBJECTS)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    s_ProtoReturnFunctions[METHOD_COMPLETE_POST_OBJECTS - 1] = ProtoReturn_CompletePostObjects;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A4878
bool nn::nex::DataStoreProtocolClient::CallGetPersistenceInfos(nn::nex::ProtocolCallContext* pContext, const u32& ownerId, const nn::nex::qVector<u16>& persistenceSlotIds, nn::nex::qVector<nn::nex::DataStorePersistenceInfo>* pPersistenceInfo, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_PERSISTENCE_INFOS)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&ownerId), sizeof(ownerId));
    _Type_qvector<_Type_uint16>::Add(&message, persistenceSlotIds);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pPersistenceInfo);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_GET_PERSISTENCE_INFOS - 1] = ProtoReturn_GetPersistenceInfos;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A4A44
bool nn::nex::DataStoreProtocolClient::CallPreparePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePreparePostParamV1& param, nn::nex::DataStoreReqPostInfoV1* pReqPostInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PREPARE_POST_OBJECT_V1)) {
        return false;
    }
    DDLClassMetaType<DataStorePreparePostParamV1>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pReqPostInfo);
    }
    s_ProtoReturnFunctions[METHOD_PREPARE_POST_OBJECT_V1 - 1] = ProtoReturn_PreparePostObjectV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A4BD8
bool nn::nex::DataStoreProtocolClient::CallPrepareUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareUpdateParam& param, nn::nex::DataStoreReqUpdateInfo* pReqUpdateInfo)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PREPARE_UPDATE_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStorePrepareUpdateParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pReqUpdateInfo);
    }
    s_ProtoReturnFunctions[METHOD_PREPARE_UPDATE_OBJECT - 1] = ProtoReturn_PrepareUpdateObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A4D38
void nn::nex::DataStoreProtocolClient::ProtoReturn_ChangeMetas(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 0);
}

// 0x003A530C
void nn::nex::DataStoreProtocolClient::ProtoReturn_RateObjects(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreRatingInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003A5E88
void nn::nex::DataStoreProtocolClient::ProtoReturn_ResetRating(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003A5E8C
void nn::nex::DataStoreProtocolClient::ProtoReturn_TouchObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003A5E98
bool nn::nex::DataStoreProtocolClient::CallCompletePostObjectV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompletePostParamV1& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_COMPLETE_POST_OBJECT_V1)) {
        return false;
    }
    DDLClassMetaType<DataStoreCompletePostParamV1>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_COMPLETE_POST_OBJECT_V1 - 1] = ProtoReturn_CompletePostObjectV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A60A4
bool nn::nex::DataStoreProtocolClient::CallCompleteUpdateObject(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreCompleteUpdateParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_COMPLETE_UPDATE_OBJECT)) {
        return false;
    }
    DDLClassMetaType<DataStoreCompleteUpdateParam>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_COMPLETE_UPDATE_OBJECT - 1] = ProtoReturn_CompleteUpdateObject;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A62B8
void nn::nex::DataStoreProtocolClient::ProtoReturn_ChangeMetaV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003A62BC
void nn::nex::DataStoreProtocolClient::ProtoReturn_DeleteObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003A62C0
void nn::nex::DataStoreProtocolClient::ProtoReturn_ResetRatings(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 0);
}

// 0x003A6894
void nn::nex::DataStoreProtocolClient::ProtoReturn_SearchObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreSearchResult>>(pMessage, pContext, 0);
}

// 0x003A6910
bool nn::nex::DataStoreProtocolClient::CallGetMetasMultipleParam(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreGetMetaParam>& params, nn::nex::qVector<nn::nex::DataStoreMetaInfo>* pMetaInfo, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_METAS_MULTIPLE_PARAM)) {
        return false;
    }
    _Type_qvector<DDLClassMetaType<DataStoreGetMetaParam> >::Add(&message, params);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pMetaInfo);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_GET_METAS_MULTIPLE_PARAM - 1] = ProtoReturn_GetMetasMultipleParam;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A6AD0
bool nn::nex::DataStoreProtocolClient::CallRateObjectWithPosting(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreRatingTarget& target, const nn::nex::DataStoreRateObjectParam& rateParam, const nn::nex::DataStorePreparePostParam& postParam, const bool& fetchRatings, nn::nex::DataStoreRatingInfo* pRating)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_RATE_OBJECT_WITH_POSTING)) {
        return false;
    }
    DDLClassMetaType<DataStoreRatingTarget>::Add(&message, target);
    DDLClassMetaType<DataStoreRateObjectParam>::Add(&message, rateParam);
    DDLClassMetaType<DataStorePreparePostParam>::Add(&message, postParam);
    message << fetchRatings;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRating);
    }
    s_ProtoReturnFunctions[METHOD_RATE_OBJECT_WITH_POSTING - 1] = ProtoReturn_RateObjectWithPosting;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A6C5C
void nn::nex::DataStoreProtocolClient::ProtoReturn_ChangeMetasV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 0);
}

// 0x003A7230
void nn::nex::DataStoreProtocolClient::ProtoReturn_DeleteObjects(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 0);
}

// 0x003A7804
bool nn::nex::DataStoreProtocolClient::CallRateObjectsWithPosting(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<nn::nex::DataStoreRatingTarget>& targets, const nn::nex::qVector<nn::nex::DataStoreRateObjectParam>& rateParams, const nn::nex::qVector<nn::nex::DataStorePreparePostParam>& postParams, const bool& transactional, const bool& fetchRatings, nn::nex::qVector<nn::nex::DataStoreRatingInfo>* pRatings, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_RATE_OBJECTS_WITH_POSTING)) {
        return false;
    }
    _Type_qvector<DDLClassMetaType<DataStoreRatingTarget> >::Add(&message, targets);
    _Type_qvector<DDLClassMetaType<DataStoreRateObjectParam> >::Add(&message, rateParams);
    _Type_qvector<DDLClassMetaType<DataStorePreparePostParam> >::Add(&message, postParams);
    message << transactional;
    message << fetchRatings;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pRatings);
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_RATE_OBJECTS_WITH_POSTING - 1] = ProtoReturn_RateObjectsWithPosting;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A7A7C
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetObjectInfos(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreReqGetInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003A8754
void nn::nex::DataStoreProtocolClient::ProtoReturn_PostMetaBinary(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_uint64>(pMessage, pContext, 0);
}

// 0x003A8858
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetPasswordInfo(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStorePasswordInfo>>(pMessage, pContext, 0);
}

// 0x003A88D4
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetSpecificMeta(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreSpecificMetaInfo> >>(pMessage, pContext, 0);
}

// 0x003A90F0
bool nn::nex::DataStoreProtocolClient::CallPostMetaBinaryWithDataId(nn::nex::ProtocolCallContext* pContext, const u64& dataId, const nn::nex::DataStorePreparePostParam& param)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_POST_META_BINARY_WITH_DATA_ID)) {
        return false;
    }
    message.AppendRaw(reinterpret_cast<const u8*>(&dataId), sizeof(dataId));
    DDLClassMetaType<DataStorePreparePostParam>::Add(&message, param);
    s_ProtoReturnFunctions[METHOD_POST_META_BINARY_WITH_DATA_ID - 1] = ProtoReturn_PostMetaBinaryWithDataId;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003A9248
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetPasswordInfos(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStorePasswordInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003A9DFC
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetRatingWithLog(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreRatingInfo>>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<DDLClassMetaType<DataStoreRatingLog>>(pMessage, pContext, 1);
}

// 0x003A9F0C
void nn::nex::DataStoreProtocolClient::ProtoReturn_PerpetuateObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003A9F10
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareGetObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqGetInfo>>(pMessage, pContext, 0);
}

// 0x003A9F8C
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetSpecificMetaV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreSpecificMetaInfoV1> >>(pMessage, pContext, 0);
}

// 0x003AA830
void nn::nex::DataStoreProtocolClient::ProtoReturn_PreparePostObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqPostInfo>>(pMessage, pContext, 0);
}

// 0x003AAA98
void nn::nex::DataStoreProtocolClient::ProtoReturn_SearchObjectLight(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreSearchResult>>(pMessage, pContext, 0);
}

// 0x003AAB14
bool nn::nex::DataStoreProtocolClient::CallGetNewArrivedNotifications(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param, nn::nex::qVector<nn::nex::DataStoreNotification>* pResult, bool* pHasNext)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_NEW_ARRIVED_NOTIFICATIONS)) {
        return false;
    }
    DDLClassMetaType<DataStoreGetNewArrivedNotificationsParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pHasNext);
    }
    s_ProtoReturnFunctions[METHOD_GET_NEW_ARRIVED_NOTIFICATIONS - 1] = ProtoReturn_GetNewArrivedNotifications;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003AAC84
bool nn::nex::DataStoreProtocolClient::CallPostMetaBinariesWithDataId(nn::nex::ProtocolCallContext* pContext, const nn::nex::qVector<u64>& dataIds, const nn::nex::qVector<nn::nex::DataStorePreparePostParam>& params, const bool& transactional, nn::nex::qVector<nn::nex::qResult>* pResults)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_POST_META_BINARIES_WITH_DATA_ID)) {
        return false;
    }
    _Type_qvector<_Type_uint64>::Add(&message, dataIds);
    _Type_qvector<DDLClassMetaType<DataStorePreparePostParam> >::Add(&message, params);
    message << transactional;
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResults);
    }
    s_ProtoReturnFunctions[METHOD_POST_META_BINARIES_WITH_DATA_ID - 1] = ProtoReturn_PostMetaBinariesWithDataId;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003AAE94
void nn::nex::DataStoreProtocolClient::ProtoReturn_CompletePostObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003AAE98
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetNotificationUrl(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqGetNotificationUrlInfo>>(pMessage, pContext, 0);
}

// 0x003AB0D0
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetPersistenceInfo(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStorePersistenceInfo>>(pMessage, pContext, 0);
}

// 0x003AB14C
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareGetObjectV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqGetInfoV1>>(pMessage, pContext, 0);
}

// 0x003AB384
void nn::nex::DataStoreProtocolClient::ProtoReturn_UnperpetuateObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003AB388
void nn::nex::DataStoreProtocolClient::ProtoReturn_CompletePostObjects(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003AB38C
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetPersistenceInfos(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStorePersistenceInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003ABFA8
void nn::nex::DataStoreProtocolClient::ProtoReturn_PreparePostObjectV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqPostInfoV1>>(pMessage, pContext, 0);
}

// 0x003AC210
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareUpdateObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqUpdateInfo>>(pMessage, pContext, 0);
}

// 0x003AC478
bool nn::nex::DataStoreProtocolClient::CallGetNewArrivedNotificationsV1(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStoreGetNewArrivedNotificationsParam& param, nn::nex::qVector<nn::nex::DataStoreNotificationV1>* pResult, bool* pHasNext)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_GET_NEW_ARRIVED_NOTIFICATIONS_V1)) {
        return false;
    }
    DDLClassMetaType<DataStoreGetNewArrivedNotificationsParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pResult);
        pContext->AddReturnValue(pHasNext);
    }
    s_ProtoReturnFunctions[METHOD_GET_NEW_ARRIVED_NOTIFICATIONS_V1 - 1] = ProtoReturn_GetNewArrivedNotificationsV1;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003AC5E8
bool nn::nex::DataStoreProtocolClient::CallPrepareGetObjectOrMetaBinary(nn::nex::ProtocolCallContext* pContext, const nn::nex::DataStorePrepareGetParam& param, nn::nex::DataStoreReqGetInfo* pReqGetInfo, nn::nex::DataStoreReqGetAdditionalMeta* pReqGetAdditionalMeta)
{
    Message message;
    CriticalSection& lock = GetLock();
    if (!BeginRegistered(this, &message, lock, pContext, METHOD_PREPARE_GET_OBJECT_OR_META_BINARY)) {
        return false;
    }
    DDLClassMetaType<DataStorePrepareGetParam>::Add(&message, param);
    if (pContext != nullptr) {
        pContext->AddReturnValue(pReqGetInfo);
        pContext->AddReturnValue(pReqGetAdditionalMeta);
    }
    s_ProtoReturnFunctions[METHOD_PREPARE_GET_OBJECT_OR_META_BINARY - 1] = ProtoReturn_PrepareGetObjectOrMetaBinary;
    return EndRegistered(this, &message, lock, pContext);
}

// 0x003AC758
void nn::nex::DataStoreProtocolClient::ProtoReturn_CompletePostObjectV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003AC75C
void nn::nex::DataStoreProtocolClient::ProtoReturn_CompleteUpdateObject(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003AC760
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetMetasMultipleParam(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreMetaInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003AD390
void nn::nex::DataStoreProtocolClient::ProtoReturn_RateObjectWithPosting(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreRatingInfo>>(pMessage, pContext, 0);
}

// 0x003AD40C
void nn::nex::DataStoreProtocolClient::ProtoReturn_RateObjectsWithPosting(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreRatingInfo> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 1);
}

// 0x003ADF88
void nn::nex::DataStoreProtocolClient::ProtoReturn_PostMetaBinaryWithDataId(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    // nothing to do: the method has no results
}

// 0x003ADF8C
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetNewArrivedNotifications(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreNotification> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<BoolType>(pMessage, pContext, 1);
}

// 0x003AE7C4
void nn::nex::DataStoreProtocolClient::ProtoReturn_PostMetaBinariesWithDataId(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<_Type_qresult>>(pMessage, pContext, 0);
}

// 0x003AED98
void nn::nex::DataStoreProtocolClient::ProtoReturn_GetNewArrivedNotificationsV1(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<_Type_qvector<DDLClassMetaType<DataStoreNotificationV1> >>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<BoolType>(pMessage, pContext, 1);
}

// 0x003AF5B8
void nn::nex::DataStoreProtocolClient::ProtoReturn_PrepareGetObjectOrMetaBinary(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    ExtractReturn<DDLClassMetaType<DataStoreReqGetInfo>>(pMessage, pContext, 0);
    if (pMessage->m_IsError) {
        return;
    }
    ExtractReturn<DDLClassMetaType<DataStoreReqGetAdditionalMeta>>(pMessage, pContext, 1);
}

// 0x003A79F8 slot 0x50
void nn::nex::DataStoreProtocolClient::ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext)
{
    u32 methodId = Protocol::ExtractMethodID(pMessage) & 0x7FFF;
    if (methodId == 0 || methodId > METHOD_COUNT) {
        SetCallError(qResult(RESULT_UNKNOWN_METHOD));
        return;
    }
    ProtoReturnFunction pFunction = s_ProtoReturnFunctions[methodId - 1];
    if (pFunction != nullptr) {
        pFunction(pMessage, pContext);
    }
}

// 0x003AF880
nn::nex::DataStoreProtocolClient::DataStoreProtocolClient(unsigned short id) : ClientProtocol(FLAG_REGISTER)
{
    if (id != 0) {
        m_ProtocolId = id;
        DataStoreProtocolClient* pResponder = new DataStoreProtocolClient(0);
        RVClientCore::GetInstance()->m_pProtocolRegistry->RegisterProtocol(id, pResponder);
    }
}

// 0x003AF978 slot 0x00
// 0x003AF968 (deleting dtor)
nn::nex::DataStoreProtocolClient::~DataStoreProtocolClient()
{
    // nothing to do
}

// 0x0072D0E4 slot 0x54 | fefates:callseq
nn::nex::ClientProtocol* nn::nex::DataStoreProtocolClient::CreateResponder() const
{
    return new DataStoreProtocolClient(0);
}

} // namespace nex
} // namespace nn
