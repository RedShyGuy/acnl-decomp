#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreCompletePostParam.h"
#include "nn/nex/nex_DataStoreCompletePostParamV1.h"
#include "nn/nex/nex_DataStoreGetSpecificMetaParam.h"
#include "nn/nex/nex_DataStoreGetSpecificMetaParamV1.h"
#include "nn/nex/nex_DataStoreNotification.h"
#include "nn/nex/nex_DataStoreNotificationV1.h"
#include "nn/nex/nex_DataStorePrepareGetParam.h"
#include "nn/nex/nex_DataStorePrepareGetParamV1.h"
#include "nn/nex/nex_DataStorePreparePostParam.h"
#include "nn/nex/nex_DataStorePreparePostParamV1.h"
#include "nn/nex/nex_DataStoreReqGetInfo.h"
#include "nn/nex/nex_DataStoreReqGetInfoV1.h"
#include "nn/nex/nex_DataStoreReqPostInfo.h"
#include "nn/nex/nex_DataStoreReqPostInfoV1.h"
#include "nn/nex/nex_DataStoreSpecificMetaInfo.h"
#include "nn/nex/nex_DataStoreSpecificMetaInfoV1.h"

namespace nn {
namespace nex {
// The versions of the DataStore API of the client (the enum is from the template arguments in the
// binary, the names of the values are ours). Version 0 uses the calls of the protocol with 32-bit
// data IDs (the "V1" methods), version 1 the ones with 64-bit data IDs.
enum DATASTORE_API_VERSION
{
    DATASTORE_API_VERSION_0 = 0,
    DATASTORE_API_VERSION_1 = 1,
};

// The types of a version of the DataStore API (no code; TPreparePostParam and TDataId are from the
// signatures in the binary, the other names are ours; TVersion is the type of the version of the
// meta information).
template <DATASTORE_API_VERSION Version>
struct DataStoreApiVersionTraits;

template <>
struct DataStoreApiVersionTraits<DATASTORE_API_VERSION_0>
{
    typedef u32 TDataId;
    typedef u16 TVersion;
    typedef DataStorePrepareGetParamV1 TPrepareGetParam;
    typedef DataStoreReqGetInfoV1 TReqGetInfo;
    typedef DataStorePreparePostParamV1 TPreparePostParam;
    typedef DataStoreReqPostInfoV1 TReqPostInfo;
    typedef DataStoreCompletePostParamV1 TCompletePostParam;
    typedef DataStoreGetSpecificMetaParamV1 TGetSpecificMetaParam;
    typedef DataStoreSpecificMetaInfoV1 TSpecificMetaInfo;
    typedef DataStoreNotificationV1 TNotification;
};

template <>
struct DataStoreApiVersionTraits<DATASTORE_API_VERSION_1>
{
    typedef u64 TDataId;
    typedef u32 TVersion;
    typedef DataStorePrepareGetParam TPrepareGetParam;
    typedef DataStoreReqGetInfo TReqGetInfo;
    typedef DataStorePreparePostParam TPreparePostParam;
    typedef DataStoreReqPostInfo TReqPostInfo;
    typedef DataStoreCompletePostParam TCompletePostParam;
    typedef DataStoreGetSpecificMetaParam TGetSpecificMetaParam;
    typedef DataStoreSpecificMetaInfo TSpecificMetaInfo;
    typedef DataStoreNotification TNotification;
};

typedef DataStoreApiVersionTraits<DATASTORE_API_VERSION_0> DataStoreApiVersionTraitsV0;
typedef DataStoreApiVersionTraits<DATASTORE_API_VERSION_1> DataStoreApiVersionTraitsV1;
} // namespace nex
} // namespace nn
