#include "nn/nex/nex_DataStoreSpecificMetaInfoV1.h"

namespace nn {
namespace nex {
// 0x003BCFE8 slot 0x00
// 0x003BCFE4 (deleting dtor)
nn::nex::DataStoreSpecificMetaInfoV1::~DataStoreSpecificMetaInfoV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008335A0
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreSpecificMetaInfoV1, nn::nex::MemAllocator<nn::nex::DataStoreSpecificMetaInfoV1> >::_C_insert_aux(nn::nex::DataStoreSpecificMetaInfoV1* pos, const nn::nex::DataStoreSpecificMetaInfoV1& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
