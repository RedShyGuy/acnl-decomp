#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreNotification.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21DataStoreNotificationE @ 0x008CEA40
// vtable 0x008FDC9C (vptr 0x008FDCA4), offset_to_top 0, 2 entries
class DataStoreNotification : public ::nn::nex::_DDL_DataStoreNotification
{
public:
    // (inline)
    DataStoreNotification() {}
    virtual ~DataStoreNotification(); // 0x003983FC slot 0x00
    // 0x003983F8 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreNotification, 0x18);
} // namespace nex
} // namespace nn

namespace std {
// (out of line)
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreNotification, nn::nex::MemAllocator<nn::nex::DataStoreNotification> >::_C_insert_aux(nn::nex::DataStoreNotification* pos, const nn::nex::DataStoreNotification& value);
} // namespace std
