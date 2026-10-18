#include "nn/nex/nex_DataStoreMetaInfo.h"

namespace nn {
namespace nex {
// 0x00386C34
nn::nex::DataStoreMetaInfo::DataStoreMetaInfo()
{
    // nothing to do
}

// 0x00386B38
nn::nex::DataStoreMetaInfo::DataStoreMetaInfo(const nn::nex::DataStoreMetaInfo& other) : _DDL_DataStoreMetaInfo(other)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00386D44 slot 0x00
// 0x00386D34 (deleting dtor)
nn::nex::DataStoreMetaInfo::~DataStoreMetaInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00830D78
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreMetaInfo, nn::nex::MemAllocator<nn::nex::DataStoreMetaInfo> >::_C_insert_aux(nn::nex::DataStoreMetaInfo* pos, const nn::nex::DataStoreMetaInfo& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
