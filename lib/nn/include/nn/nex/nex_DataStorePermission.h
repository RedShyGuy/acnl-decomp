#pragma once

#include "decomp.h"
#include "nn/nex/nex_DataStoreConstants.h"
#include "nn/nex/nex__DDL_DataStorePermission.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19DataStorePermissionE @ 0x008CE7B4
// vtable 0x008FD670 (vptr 0x008FD678), offset_to_top 0, 2 entries
class DataStorePermission : public ::nn::nex::_DDL_DataStorePermission
{
public:
    // (inline)
    DataStorePermission() {}
    DataStorePermission(nn::nex::DataStoreConstants::Permission permission, const nn::nex::qVector<unsigned int>& recipientIds); // 0x00392DE0 | fefates:bytes [tier B]
    DataStorePermission(const nn::nex::DataStorePermission& other); // 0x00392E50
    virtual ~DataStorePermission(); // 0x00392F3C slot 0x00
    // 0x00392F14 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStorePermission, 0x14);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStorePermission>::Add(Message* pMessage, const nn::nex::DataStorePermission& value);
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStorePermission>::Extract(Message* pMessage, nn::nex::DataStorePermission* pValue);
} // namespace nex
} // namespace nn
