#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreDeleteParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20DataStoreDeleteParamE @ 0x008CE868
// vtable 0x008FD7F4 (vptr 0x008FD7FC), offset_to_top 0, 2 entries
class DataStoreDeleteParam : public ::nn::nex::_DDL_DataStoreDeleteParam
{
public:
    // (inline)
    DataStoreDeleteParam() {}
    virtual ~DataStoreDeleteParam(); // 0x003962B0 slot 0x00
    // 0x003962AC slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreDeleteParam, 0x18);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreDeleteParam>::Add(Message* pMessage, const nn::nex::DataStoreDeleteParam& value);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreDeleteParam, nn::nex::MemAllocator<nn::nex::DataStoreDeleteParam> >::_C_insert_aux(nn::nex::DataStoreDeleteParam* pos, const nn::nex::DataStoreDeleteParam& value);
} // namespace std
