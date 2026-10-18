#include "nn/nex/nex_DataStoreChangeMetaParamV1.h"

namespace nn {
namespace nex {
// 0x003B8B30 slot 0x00
// 0x003B8B20 (deleting dtor)
nn::nex::DataStoreChangeMetaParamV1::~DataStoreChangeMetaParamV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003C6B3C
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreChangeMetaParamV1>::Add(Message* pMessage, const nn::nex::DataStoreChangeMetaParamV1& value)
{
    DataStoreChangeMetaParamV1::Add(pMessage, value);
}

// 0x007D55E4
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreChangeMetaParamV1> >::Add(Message* pMessage, const Type& vector)
{
    AddInline(pMessage, vector);
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00832A68
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreChangeMetaParamV1, nn::nex::MemAllocator<nn::nex::DataStoreChangeMetaParamV1> >::_C_insert_aux(nn::nex::DataStoreChangeMetaParamV1* pos, const nn::nex::DataStoreChangeMetaParamV1& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
