#include "nn/nex/nex_DataStoreRateObjectParam.h"

namespace nn {
namespace nex {
// 0x003B24B4 slot 0x00
// 0x003B24B0 (deleting dtor)
nn::nex::DataStoreRateObjectParam::~DataStoreRateObjectParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x007D54C0 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreRateObjectParam>>::Add(Message* pMessage, const Type& vector)
{
    AddInline(pMessage, vector);
}

// 0x003C1A94
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreRateObjectParam>::Add(Message* pMessage, const nn::nex::DataStoreRateObjectParam& value)
{
    DataStoreRateObjectParam::Add(pMessage, value);
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008322CC
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreRateObjectParam, nn::nex::MemAllocator<nn::nex::DataStoreRateObjectParam> >::_C_insert_aux(nn::nex::DataStoreRateObjectParam* pos, const nn::nex::DataStoreRateObjectParam& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std

namespace std {
// 0x00832528
template <>
DECOMP_NOINLINE vector<nn::nex::DataStoreRateObjectParam, nn::nex::MemAllocator<nn::nex::DataStoreRateObjectParam> >::~vector()
{
    DestroyInline();
}
} // namespace std
