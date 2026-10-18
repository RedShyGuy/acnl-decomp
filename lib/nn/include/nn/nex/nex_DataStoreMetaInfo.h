#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreMetaInfo.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17DataStoreMetaInfoE @ 0x008CE6A4
// vtable 0x008FD3C8 (vptr 0x008FD3D0), offset_to_top 0, 2 entries
class DataStoreMetaInfo : public ::nn::nex::_DDL_DataStoreMetaInfo
{
public:
    DataStoreMetaInfo(); // 0x00386C34
    DataStoreMetaInfo(const nn::nex::DataStoreMetaInfo& other); // 0x00386B38
    virtual ~DataStoreMetaInfo(); // 0x00386D44 slot 0x00
    // 0x00386D34 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreMetaInfo, 0xB8);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreMetaInfo, nn::nex::MemAllocator<nn::nex::DataStoreMetaInfo> >::_C_insert_aux(nn::nex::DataStoreMetaInfo* pos, const nn::nex::DataStoreMetaInfo& value);
} // namespace std
