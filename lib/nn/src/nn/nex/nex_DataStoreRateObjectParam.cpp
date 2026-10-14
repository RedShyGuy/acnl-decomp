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

} // namespace nex
} // namespace nn
