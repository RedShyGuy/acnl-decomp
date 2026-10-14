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
