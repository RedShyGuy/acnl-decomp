#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreGetNewArrivedNotificationsParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex40DataStoreGetNewArrivedNotificationsParamE @ 0x008CF488
// vtable 0x008FF8D0 (vptr 0x008FF8D8), offset_to_top 0, 2 entries
class DataStoreGetNewArrivedNotificationsParam : public ::nn::nex::_DDL_DataStoreGetNewArrivedNotificationsParam
{
public:
    // (inline)
    DataStoreGetNewArrivedNotificationsParam() {}
    virtual ~DataStoreGetNewArrivedNotificationsParam(); // 0x003CD06C slot 0x00
    // 0x003CD068 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreGetNewArrivedNotificationsParam, 0x18);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreGetNewArrivedNotificationsParam>::Add(Message* pMessage, const nn::nex::DataStoreGetNewArrivedNotificationsParam& value);
} // namespace nex
} // namespace nn
