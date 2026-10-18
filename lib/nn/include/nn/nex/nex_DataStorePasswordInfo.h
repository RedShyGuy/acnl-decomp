#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStorePasswordInfo.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21DataStorePasswordInfoE @ 0x008CEA4C
// vtable 0x008FDCAC (vptr 0x008FDCB4), offset_to_top 0, 2 entries
class DataStorePasswordInfo : public ::nn::nex::_DDL_DataStorePasswordInfo
{
public:
    DataStorePasswordInfo(); // 0x00398400
    virtual ~DataStorePasswordInfo(); // 0x00398424 slot 0x00
    // 0x00398420 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStorePasswordInfo, 0x20);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStorePasswordInfo, nn::nex::MemAllocator<nn::nex::DataStorePasswordInfo> >::_C_insert_aux(nn::nex::DataStorePasswordInfo* pos, const nn::nex::DataStorePasswordInfo& value);
} // namespace std
