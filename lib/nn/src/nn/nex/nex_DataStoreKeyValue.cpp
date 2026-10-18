#include "nn/nex/nex_DataStoreKeyValue.h"

namespace nn {
namespace nex {
// 0x00386B14 slot 0x00
// 0x00386AEC (deleting dtor)
nn::nex::DataStoreKeyValue::~DataStoreKeyValue()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x007D4FA4 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Extract(Message* pMessage, Type* pVector)
{
    ExtractInline(pMessage, pVector);
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00830804
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreKeyValue, nn::nex::MemAllocator<nn::nex::DataStoreKeyValue> >::_C_insert_aux(nn::nex::DataStoreKeyValue* pos, const nn::nex::DataStoreKeyValue& value)
{
    InsertAuxInline(pos, value);
}
// 0x00830A9C (symbols.json names it the constructor of DataStoreKeyValue)
template <>
DECOMP_NOINLINE vector<nn::nex::DataStoreKeyValue, nn::nex::MemAllocator<nn::nex::DataStoreKeyValue> >& vector<nn::nex::DataStoreKeyValue, nn::nex::MemAllocator<nn::nex::DataStoreKeyValue> >::operator=(const vector& other)
{
    return AssignInline(other);
}
} // namespace std
