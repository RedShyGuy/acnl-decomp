#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreRateObjectParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24DataStoreRateObjectParamE @ 0x008CED80
// vtable 0x008FE758 (vptr 0x008FE760), offset_to_top 0, 2 entries
class DataStoreRateObjectParam : public ::nn::nex::_DDL_DataStoreRateObjectParam
{
public:
    // (inline)
    DataStoreRateObjectParam() {}
    virtual ~DataStoreRateObjectParam(); // 0x003B24B4 slot 0x00
    // 0x003B24B0 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreRateObjectParam, 0x18);

// (out of line)
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreRateObjectParam>>::Add(Message* pMessage, const Type& vector);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreRateObjectParam>::Add(Message* pMessage, const nn::nex::DataStoreRateObjectParam& value);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreRateObjectParam, nn::nex::MemAllocator<nn::nex::DataStoreRateObjectParam> >::_C_insert_aux(nn::nex::DataStoreRateObjectParam* pos, const nn::nex::DataStoreRateObjectParam& value);
} // namespace std

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE vector<nn::nex::DataStoreRateObjectParam, nn::nex::MemAllocator<nn::nex::DataStoreRateObjectParam> >::~vector();
} // namespace std
