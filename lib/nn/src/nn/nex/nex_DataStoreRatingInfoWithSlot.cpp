#include "nn/nex/nex_DataStoreRatingInfoWithSlot.h"

namespace nn {
namespace nex {
// 0x003BCFD4 slot 0x00
// 0x003BCFC4 (deleting dtor)
nn::nex::DataStoreRatingInfoWithSlot::~DataStoreRatingInfoWithSlot()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x007D5798 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreRatingInfoWithSlot>>::Extract(Message* pMessage, Type* pVector)
{
    ExtractInline(pMessage, pVector);
}

// 0x003D44C8 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qVector<qVector<DataStoreRatingInfoWithSlot> >::~qVector()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008331E8 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE vector<nn::nex::DataStoreRatingInfoWithSlot, nn::nex::MemAllocator<nn::nex::DataStoreRatingInfoWithSlot> >::~vector()
{
    DestroyInline();
}
} // namespace std
