#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStorePermission.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19DataStorePermissionE @ 0x008CE7B4
// vtable 0x008FD670 (vptr 0x008FD678), offset_to_top 0, 2 entries
class DataStorePermission : public ::nn::nex::_DDL_DataStorePermission
{
public:
    DataStorePermission(); // ctor candidate(s) 0x00392DE0, 0x00392E60 (unverified)
    virtual void vf_0x00(); // 0x00392F3C slot 0x00 | virtual slot, introduced by nn::nex::_DDL_DataStorePermission
    virtual void vf_0x04(); // 0x00392F14 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_DataStorePermission
    DataStorePermission(nn::nex::DataStoreConstants::Permission, const nn::nex::qVector<unsigned int>&); // 0x00392DE0 | fefates:bytes [tier B]
    DataStorePermission(const nn::nex::DataStorePermission&); // 0x00392E50 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
