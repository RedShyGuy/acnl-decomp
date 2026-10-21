#include "nn/nex/nex_DataStoreGetNewArrivedNotificationsParam.h"

namespace nn {
namespace nex {
// 0x003CD06C slot 0x00
// 0x003CD068 (deleting dtor)
nn::nex::DataStoreGetNewArrivedNotificationsParam::~DataStoreGetNewArrivedNotificationsParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x007D72C4
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreGetNewArrivedNotificationsParam>::Add(Message* pMessage, const nn::nex::DataStoreGetNewArrivedNotificationsParam& value)
{
    DataStoreGetNewArrivedNotificationsParam::Add(pMessage, value);
}

} // namespace nex
} // namespace nn
