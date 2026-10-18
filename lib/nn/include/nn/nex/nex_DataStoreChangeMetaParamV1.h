#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreChangeMetaParamV1.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex26DataStoreChangeMetaParamV1E @ 0x008CEF18
// vtable 0x008FEB8C (vptr 0x008FEB94), offset_to_top 0, 2 entries
class DataStoreChangeMetaParamV1 : public ::nn::nex::_DDL_DataStoreChangeMetaParamV1
{
public:
    // (inline)
    DataStoreChangeMetaParamV1() {}
    virtual ~DataStoreChangeMetaParamV1(); // 0x003B8B30 slot 0x00
    // 0x003B8B20 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreChangeMetaParamV1, 0x80);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreChangeMetaParamV1>::Add(Message* pMessage, const nn::nex::DataStoreChangeMetaParamV1& value);

// (out of line)
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreChangeMetaParamV1> >::Add(Message* pMessage, const Type& vector);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreChangeMetaParamV1, nn::nex::MemAllocator<nn::nex::DataStoreChangeMetaParamV1> >::_C_insert_aux(nn::nex::DataStoreChangeMetaParamV1* pos, const nn::nex::DataStoreChangeMetaParamV1& value);
} // namespace std
