#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreSpecificMetaInfo.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25DataStoreSpecificMetaInfoE @ 0x008CEE94
// vtable 0x008FE9D4 (vptr 0x008FE9DC), offset_to_top 0, 2 entries
class DataStoreSpecificMetaInfo : public ::nn::nex::_DDL_DataStoreSpecificMetaInfo
{
public:
    // (inline)
    DataStoreSpecificMetaInfo() {}
    virtual ~DataStoreSpecificMetaInfo(); // 0x003B6F68 slot 0x00
    // 0x003B6F64 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreSpecificMetaInfo, 0x20);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreSpecificMetaInfo, nn::nex::MemAllocator<nn::nex::DataStoreSpecificMetaInfo> >::_C_insert_aux(nn::nex::DataStoreSpecificMetaInfo* pos, const nn::nex::DataStoreSpecificMetaInfo& value);
} // namespace std
