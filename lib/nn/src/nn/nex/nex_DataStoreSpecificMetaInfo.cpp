#include "nn/nex/nex_DataStoreSpecificMetaInfo.h"

namespace nn {
namespace nex {
// 0x003B6F68 slot 0x00
// 0x003B6F64 (deleting dtor)
nn::nex::DataStoreSpecificMetaInfo::~DataStoreSpecificMetaInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008327BC
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreSpecificMetaInfo, nn::nex::MemAllocator<nn::nex::DataStoreSpecificMetaInfo> >::_C_insert_aux(nn::nex::DataStoreSpecificMetaInfo* pos, const nn::nex::DataStoreSpecificMetaInfo& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
