#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreMetaInfo.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17DataStoreMetaInfoE @ 0x008CE6A4
// vtable 0x008FD3C8 (vptr 0x008FD3D0), offset_to_top 0, 2 entries
class DataStoreMetaInfo : public ::nn::nex::_DDL_DataStoreMetaInfo
{
public:
    DataStoreMetaInfo(); // ctor candidate(s) 0x00386B38, 0x00386C34, 0x003A1C48, 0x003AC760, 0x003BBE30 (unverified)
    virtual ~DataStoreMetaInfo(); // 0x00386D44 slot 0x00 | slot vf_0x00 of nn::nex::_DDL_DataStoreMetaInfo
    // 0x00386D34 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_DataStoreMetaInfo (deleting dtor)
    DataStoreMetaInfo(const nn::nex::DataStoreMetaInfo&); // 0x00386B38 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
