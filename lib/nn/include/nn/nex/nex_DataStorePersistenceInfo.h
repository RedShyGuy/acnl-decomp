#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStorePersistenceInfo.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24DataStorePersistenceInfoE @ 0x008CED68
// vtable 0x008FE738 (vptr 0x008FE740), offset_to_top 0, 2 entries
class DataStorePersistenceInfo : public ::nn::nex::_DDL_DataStorePersistenceInfo
{
public:
    DataStorePersistenceInfo(); // 0x003B2320
    virtual ~DataStorePersistenceInfo(); // 0x003B2344 slot 0x00
    // 0x003B2340 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStorePersistenceInfo, 0x18);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStorePersistenceInfo, nn::nex::MemAllocator<nn::nex::DataStorePersistenceInfo> >::_C_insert_aux(nn::nex::DataStorePersistenceInfo* pos, const nn::nex::DataStorePersistenceInfo& value);
} // namespace std
