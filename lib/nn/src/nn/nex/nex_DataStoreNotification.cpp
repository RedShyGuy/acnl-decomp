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
