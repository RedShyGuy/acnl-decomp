#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreRatingInfo.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19DataStoreRatingInfoE @ 0x008CE7C0
// vtable 0x008FD680 (vptr 0x008FD688), offset_to_top 0, 2 entries
class DataStoreRatingInfo : public ::nn::nex::_DDL_DataStoreRatingInfo
{
public:
    DataStoreRatingInfo(); // 0x00392F60
    virtual ~DataStoreRatingInfo(); // 0x00392F7C slot 0x00
    // 0x00392F78 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreRatingInfo, 0x20);

// (out of line in the original; nex_DataStoreRatingInfo.cpp)
template <>
DECOMP_NOINLINE qMap<s8, DataStoreRatingInfo>::qMap(const qMap<s8, DataStoreRatingInfo>& other);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreRatingInfo, nn::nex::MemAllocator<nn::nex::DataStoreRatingInfo> >::_C_insert_aux(nn::nex::DataStoreRatingInfo* pos, const nn::nex::DataStoreRatingInfo& value);
} // namespace std
