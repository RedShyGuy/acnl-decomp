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
