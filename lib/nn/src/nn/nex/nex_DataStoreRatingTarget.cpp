#include "nn/nex/nex_DataStoreRatingTarget.h"

namespace nn {
namespace nex {
// 0x0039842C slot 0x00
// 0x00398428 (deleting dtor)
nn::nex::DataStoreRatingTarget::~DataStoreRatingTarget()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x007D539C | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreRatingTarget>>::Add(Message* pMessage, const Type& vector)
{
    AddInline(pMessage, vector);
}

} // namespace nex
} // namespace nn
