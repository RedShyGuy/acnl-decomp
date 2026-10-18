#include "nn/nex/nex_DataStorePasswordInfo.h"

namespace nn {
namespace nex {
// 0x00398400
nn::nex::DataStorePasswordInfo::DataStorePasswordInfo()
{
    // nothing to do
}

// 0x00398424 slot 0x00
// 0x00398420 (deleting dtor)
nn::nex::DataStorePasswordInfo::~DataStorePasswordInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008319A4
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStorePasswordInfo, nn::nex::MemAllocator<nn::nex::DataStorePasswordInfo> >::_C_insert_aux(nn::nex::DataStorePasswordInfo* pos, const nn::nex::DataStorePasswordInfo& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
