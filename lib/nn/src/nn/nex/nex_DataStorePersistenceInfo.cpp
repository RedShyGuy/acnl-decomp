#include "nn/nex/nex_DataStorePersistenceInfo.h"

namespace nn {
namespace nex {
// 0x003B2320
nn::nex::DataStorePersistenceInfo::DataStorePersistenceInfo()
{
    // nothing to do
}

// 0x003B2344 slot 0x00
// 0x003B2340 (deleting dtor)
nn::nex::DataStorePersistenceInfo::~DataStorePersistenceInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00832044
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStorePersistenceInfo, nn::nex::MemAllocator<nn::nex::DataStorePersistenceInfo> >::_C_insert_aux(nn::nex::DataStorePersistenceInfo* pos, const nn::nex::DataStorePersistenceInfo& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
