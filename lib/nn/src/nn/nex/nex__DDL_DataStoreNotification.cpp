#include "nn/nex/nex__DDL_DataStoreNotification.h"
#include "nn/nex/nex_DataStoreNotification.h"

namespace nn {
namespace nex {
// 0x003BBBFC slot 0x00
// 0x003BBBF8 (deleting dtor)
nn::nex::_DDL_DataStoreNotification::~_DDL_DataStoreNotification()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
