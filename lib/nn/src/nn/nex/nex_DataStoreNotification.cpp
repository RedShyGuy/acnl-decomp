#include "nn/nex/nex_DataStoreNotification.h"

namespace nn {
namespace nex {
// 0x003983FC slot 0x00
// 0x003983F8 (deleting dtor)
nn::nex::DataStoreNotification::~DataStoreNotification()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00831734
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreNotification, nn::nex::MemAllocator<nn::nex::DataStoreNotification> >::_C_insert_aux(nn::nex::DataStoreNotification* pos, const nn::nex::DataStoreNotification& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
