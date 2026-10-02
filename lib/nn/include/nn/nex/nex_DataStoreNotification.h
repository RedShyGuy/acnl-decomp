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
    DataStoreNotification(); // ctor candidate(s) 0x003ADF8C, 0x00831734 (unverified)
    virtual void vf_0x00(); // 0x003983FC slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStoreNotification
    virtual void vf_0x04(); // 0x003983F8 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStoreNotification
};
} // namespace nex
} // namespace nn
