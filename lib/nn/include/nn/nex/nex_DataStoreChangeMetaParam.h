#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreChangeMetaParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24DataStoreChangeMetaParamE @ 0x008CED48
// vtable 0x008FE714 (vptr 0x008FE71C), offset_to_top 0, 2 entries
class DataStoreChangeMetaParam : public ::nn::nex::_DDL_DataStoreChangeMetaParam
{
public:
    DataStoreChangeMetaParam(); // 0x003B2084
    virtual ~DataStoreChangeMetaParam(); // 0x003B22E0 slot 0x00
    // 0x003B22D0 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreChangeMetaParam, 0x110);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreChangeMetaParam>::Add(Message* pMessage, const nn::nex::DataStoreChangeMetaParam& value);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreChangeMetaParam, nn::nex::MemAllocator<nn::nex::DataStoreChangeMetaParam> >::_C_insert_aux(nn::nex::DataStoreChangeMetaParam* pos, const nn::nex::DataStoreChangeMetaParam& value);
} // namespace std
