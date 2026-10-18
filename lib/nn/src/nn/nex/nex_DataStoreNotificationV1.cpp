#include "nn/nex/nex_DataStoreNotificationV1.h"

namespace nn {
namespace nex {
// 0x0039F4A8 slot 0x00
// 0x0039F4A4 (deleting dtor)
nn::nex::DataStoreNotificationV1::~DataStoreNotificationV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x00831C1C
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreNotificationV1, nn::nex::MemAllocator<nn::nex::DataStoreNotificationV1> >::_C_insert_aux(nn::nex::DataStoreNotificationV1* pos, const nn::nex::DataStoreNotificationV1& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
